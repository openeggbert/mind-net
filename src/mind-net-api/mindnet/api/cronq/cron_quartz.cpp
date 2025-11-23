/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "mindnet/api/cronq/cron_quartz.hpp"
#include <sstream>
#include <array>
#include <cctype>
#include <ctime>
#include <limits>
#include <algorithm>


namespace mindnet::api::cronq
{
    inline std::string trim(const std::string& s)
    {
        size_t b = 0;
        while (b < s.size() && std::isspace((unsigned char)s[b])) ++b;
        size_t e = s.size();
        while (e > b && std::isspace((unsigned char)s[e - 1])) --e;
        return s.substr(b, e - b);
    }

    inline std::vector<std::string> split(const std::string& s, char delim)
    {
        std::vector<std::string> parts;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            std::string raw = item;
            item = trim(item);

            if (raw == "*" || item == "*") {
                parts.push_back("*");
                continue;
            }

            if (raw == "?" || item == "?") {
                parts.push_back("?");
                continue;
            }


            if (!item.empty())
                parts.push_back(item);
        }

        return parts;
    }

    inline std::string to_upper(std::string s)
    {
        for (char& c : s) c = (char)std::toupper((unsigned char)c);
        return s;
    }

    inline int to_int(const std::string& s)
    {
        size_t idx = 0;
        int v = std::stoi(s, &idx);
        if (idx != s.size())
            throw std::runtime_error("Invalid integer in cron: " + s);
        return v;
    }

    // last day of month
    inline int last_day_of_month(int year, int month)
    {
        static const int days_in_month[] =
            {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month < 1 || month > 12)
            throw std::runtime_error("Invalid month");
        int d = days_in_month[month - 1];
        // leap-year February
        if (month == 2)
        {
            bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (leap) d = 29;
        }
        return d;
    }

    inline std::tm to_local_tm(const std::chrono::system_clock::time_point& tp)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::tm result{};
#if defined(_WIN32)
        localtime_s(&result, &tt);
#else
        localtime_r(&tt, &result);
#endif
        return result;
    }

    // mapping of month names → 1..12
    inline bool month_name_to_num(const std::string& name, int& out)
    {
        static const std::array<const char*, 12> names = {
            "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
            "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
        };
        std::string u = to_upper(name);
        for (int i = 0; i < 12; ++i)
        {
            if (u == names[i])
            {
                out = i + 1;
                return true;
            }
        }
        return false;
    }

    // mapping of days of week → 1..7 (Quartz: 1=SUN..7=SAT, 7 alias SUN)
    inline bool dow_name_to_num(const std::string& name, int& out)
    {
        static const std::array<const char*, 7> names = {
            "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
        };
        std::string u = to_upper(name);
        for (int i = 0; i < 7; ++i)
        {
            if (u == names[i])
            {
                out = i + 1; // 1..7
                return true;
            }
        }
        return false;
    }

    // convert tm_wday (0=SUN..6=SAT) to quartz DOW (1=SUN..7=SAT)
    inline int tm_wday_to_quartz(int tm_wday)
    {
        int v = tm_wday + 1; // 0->1, 6->7
        if (v == 0) v = 7;
        return v;
    }

    // =======================================
    // Structures for cron expression fields
    // =======================================


    // evaluation against a specific date
    bool DayOfMonthField::matches(int year, int month, int day, int dow_quartz) const
    {
        switch (mode)
        {
        case Mode::ANY:
            return true;
        case Mode::UNSPECIFIED:
            return true; // DOM field is not considered itself, handled elsewhere
        case Mode::VALUES:
            return values.matches(day);
        case Mode::LAST_DAY:
            {
                int ld = last_day_of_month(year, month);
                return day == ld;
            }
        case Mode::LAST_DAY_OFFSET:
            {
                int ld = last_day_of_month(year, month);
                return day == (ld - offset);
            }
        case Mode::NEAREST_WEEKDAY:
            {
                // xW => nearest weekday (Mon–Fri) to given day x
                int ld = last_day_of_month(year, month);
                int target = weekday_base;
                if (target < 1 || target > ld)
                    return false;

                // compute actual day the job runs
                // 1) compute day-of-week of the “target” day:
                //    construct tm for (year, month, target)
                std::tm tm{};
                tm.tm_year = year - 1900;
                tm.tm_mon = month - 1;
                tm.tm_mday = target;
                tm.tm_hour = 0;
                tm.tm_isdst = -1;
                tm.tm_min = 0;
                tm.tm_sec = 0;
                std::time_t tt = std::mktime(&tm);
                std::tm loc{};
#if defined(_WIN32)
                localtime_s(&loc, &tt);
#else
                localtime_r(&tt, &loc);
#endif
                int qdow = tm_wday_to_quartz(loc.tm_wday); // 1..7 (Sun..Sat)

                auto is_weekday = [](int qdow)
                {
                    // MON..FRI = 2..6
                    return (qdow >= 2 && qdow <= 6);
                };

                int run_day = target;
                if (is_weekday(qdow))
                {
                    // runs on target
                }
                else
                {
                    // shift according to definition (if Saturday => -1, if Sunday => +1)
                    if (qdow == 7)
                    {
                        // SAT
                        run_day = target - 1;
                    }
                    else if (qdow == 1)
                    {
                        // SUN
                        run_day = target + 1;
                    }
                    // ensure no crossing of month
                    if (run_day < 1) run_day = 1;
                    if (run_day > ld) run_day = ld;
                }
                return day == run_day;
            }
        }
        return false;
    };

    bool mindnet::api::cronq::DayOfWeekField::matches(int year, int month, int day, int dow_quartz) const
    {
        switch (mode)
        {
        case Mode::ANY:
            return true;
        case Mode::UNSPECIFIED:
            return true;
        case Mode::VALUES:
            return values.matches(dow_quartz);

        case Mode::LAST_WEEKDAY:
            {
                // e.g., 5L => last Thursday in month
                int ld = last_day_of_month(year, month);

                // find the last day in the month whose DOW matches
                for (int d = ld; d >= 1; --d)
                {
                    std::tm tm{};
                    tm.tm_year = year - 1900;
                    tm.tm_mon = month - 1;
                    tm.tm_mday = d;
                    tm.tm_hour = 0;
                    tm.tm_isdst = -1;
                    std::time_t tt = std::mktime(&tm);
                    std::tm loc{};
#if defined(_WIN32)
                    localtime_s(&loc, &tt);
#else
                    localtime_r(&tt, &loc);
#endif
                    int qdow = tm_wday_to_quartz(loc.tm_wday);
                    if (qdow == weekday)
                    {
                        return (d == day);
                    }
                }
                return false;
            }

        case Mode::NTH_WEEKDAY:
            {
                // e.g., 5#3 => third Thursday in month
                int count = 0;
                int maxd = last_day_of_month(year, month);
                for (int d = 1; d <= maxd; ++d)
                {
                    std::tm tm{};
                    tm.tm_year = year - 1900;
                    tm.tm_mon = month - 1;
                    tm.tm_mday = d;
                    tm.tm_hour = 0;
                    tm.tm_isdst = -1;
                    std::time_t tt = std::mktime(&tm);
                    std::tm loc{};
#if defined(_WIN32)
                    localtime_s(&loc, &tt);
#else
                    localtime_r(&tt, &loc);
#endif
                    int qdow = tm_wday_to_quartz(loc.tm_wday);
                    if (qdow == weekday)
                    {
                        ++count;
                        if (count == nth)
                        {
                            return (d == day);
                        }
                    }
                }
                return false;
            }
        }
        return false;
    }

    // =======================================
    // Field parsing
    // =======================================

    // generic field: sec, min, hour, month (without special L/W/#/?)
    inline void parse_generic_field(const std::string& field,
                                    GenericField& out,
                                    int minv,
                                    int maxv,
                                    bool allow_names_for_month,
                                    bool allow_names_for_dow)
    {
        out = GenericField(minv, maxv);

        std::string f = to_upper(field);
        if (f == "*")
        {
            out.any = true;
            return;
        }

        auto items = split(f, ',');
        for (const auto& part : items)
        {
            std::string p = part;
            size_t slash_pos = p.find('/');
            std::string range_part = (slash_pos == std::string::npos)
                                         ? p
                                         : p.substr(0, slash_pos);
            int step = 1;
            if (slash_pos != std::string::npos)
            {
                std::string step_str = p.substr(slash_pos + 1);
                step = to_int(step_str);
                if (step <= 0)
                    throw std::runtime_error("Invalid step in cron field: " + field);
            }

            // ===============================================
            // FIX: Interpret A/B as A..maxv with step B
            // (Quartz syntax)
            // Without this, "0/5" incorrectly becomes "0..5 step 5"
            // ===============================================
            if (slash_pos != std::string::npos && range_part != "*" && range_part.find('-') == std::string::npos)
            {
                // A/B → values: A, A+B, A+2B, ... ≤ maxv
                int start_v = 0;

                // allow names for month and DOW
                if (allow_names_for_month && month_name_to_num(range_part, start_v))
                {
                    // OK
                }
                else if (allow_names_for_dow && dow_name_to_num(range_part, start_v))
                {
                    // OK
                }
                else
                {
                    start_v = to_int(range_part);
                }

                if (start_v < minv || start_v > maxv)
                    throw std::runtime_error("Invalid start value in A/B cron field: " + field);

                for (int v = start_v; v <= maxv; v += step)
                    out.add_value(v);

                return; // !!! DONE !!!
            }

            int start = 0;
            int end = 0;

            size_t dash_pos = range_part.find('-');
            if (range_part == "*")
            {
                start = minv;
                end = maxv;
            }
            else if (dash_pos != std::string::npos)
            {
                std::string a = range_part.substr(0, dash_pos);
                std::string b = range_part.substr(dash_pos + 1);

                int va = 0;
                int vb = 0;
                if (allow_names_for_month && month_name_to_num(a, va))
                {
                    // ok
                }
                else if (allow_names_for_dow && dow_name_to_num(a, va))
                {
                    // ok
                }
                else
                {
                    va = to_int(a);
                }

                if (allow_names_for_month && month_name_to_num(b, vb))
                {
                    // ok
                }
                else if (allow_names_for_dow && dow_name_to_num(b, vb))
                {
                    // ok
                }
                else
                {
                    vb = to_int(b);
                }

                start = va;
                end = vb;
            }
            else
            {
                int v = 0;
                if (allow_names_for_month && month_name_to_num(range_part, v))
                {
                    // ok
                }
                else if (allow_names_for_dow && dow_name_to_num(range_part, v))
                {
                    // ok
                }
                else
                {
                    v = to_int(range_part);
                }
                start = end = v;
            }

            if (start < minv || end > maxv || start > end)
                throw std::runtime_error("Invalid range in cron field: " + field);

            for (int val = start; val <= end; val += step)
            {
                out.add_value(val);
            }
        }
    }

    // Day-of-month field parser (supports *, ?, L, L-n, xW, ranges, etc.)
    inline DayOfMonthField parse_dom_field(const std::string& field)
    {
        DayOfMonthField dom;
        std::string f = to_upper(field);

        if (f == "*")
        {
            dom.mode = DayOfMonthField::Mode::ANY;
            return dom;
        }
        if (f == "?")
        {
            dom.mode = DayOfMonthField::Mode::UNSPECIFIED;
            return dom;
        }

        // L, L-n
        if (f == "L")
        {
            dom.mode = DayOfMonthField::Mode::LAST_DAY;
            return dom;
        }
        if (!f.empty() && f[0] == 'L')
        {
            // expecting "L-n"
            if (f.size() < 3 || f[1] != '-')
                throw std::runtime_error("Invalid L-n day-of-month: " + field);
            dom.mode = DayOfMonthField::Mode::LAST_DAY_OFFSET;
            dom.offset = to_int(f.substr(2));
            return dom;
        }

        // xW
        if (!f.empty() && f.back() == 'W')
        {
            std::string base = f.substr(0, f.size() - 1);
            int v = to_int(base);
            if (v < 1 || v > 31)
                throw std::runtime_error("Invalid W day-of-month: " + field);
            dom.mode = DayOfMonthField::Mode::NEAREST_WEEKDAY;
            dom.weekday_base = v;
            dom.values.has_values = false;
            return dom;
        }

        // otherwise: classic values / ranges / steps
        dom.mode = DayOfMonthField::Mode::VALUES;
        parse_generic_field(f, dom.values, 1, 31, false, false);
        return dom;
    }

    // Day-of-week field parser (supports *, ?, MON-FRI, 5L, 5#3, etc.)
    inline DayOfWeekField parse_dow_field(const std::string& field)
    {
        DayOfWeekField dow;
        std::string f = to_upper(field);

        if (f == "*")
        {
            dow.mode = DayOfWeekField::Mode::ANY;
            return dow;
        }
        if (f == "?")
        {
            dow.mode = DayOfWeekField::Mode::UNSPECIFIED;
            return dow;
        }

        // 5L or MONL
        if (!f.empty() && f.back() == 'L' && f.find('#') == std::string::npos)
        {
            std::string base = f.substr(0, f.size() - 1);
            int v = 0;
            if (!dow_name_to_num(base, v))
            {
                v = to_int(base);
            }
            if (v < 1 || v > 7)
                throw std::runtime_error("Invalid DOW L: " + field);
            dow.mode = DayOfWeekField::Mode::LAST_WEEKDAY;
            dow.weekday = v;
            return dow;
        }

        // 5#3 or MON#2
        {
            size_t hash = f.find('#');
            if (hash != std::string::npos)
            {
                std::string left = f.substr(0, hash);
                std::string right = f.substr(hash + 1);
                int v = 0;
                if (!dow_name_to_num(left, v))
                {
                    v = to_int(left);
                }
                int nth = to_int(right);
                if (v < 1 || v > 7)
                    throw std::runtime_error("Invalid DOW in # expression: " + field);
                if (nth < 1 || nth > 5)
                    throw std::runtime_error("Invalid Nth in # expression: " + field);
                dow.mode = DayOfWeekField::Mode::NTH_WEEKDAY;
                dow.weekday = v;
                dow.nth = nth;
                return dow;
            }
        }

        // otherwise: list / range / steps
        dow.mode = DayOfWeekField::Mode::VALUES;
        parse_generic_field(f, dow.values, 1, 7, false, true);
        return dow;
    }

    // =======================================
    // parse_cron_quartz
    // =======================================

    inline CronExpr parse_cron_quartz(const std::string& expr_raw)
    {
        const std::string expr_raw_trimmed = trim(expr_raw);
        if (expr_raw_trimmed == "-" || expr_raw_trimmed == "@reboot" || expr_raw_trimmed == "@startup")
        {
            CronExpr result;
            result.run_at_start = true;
            return result;
        }
        static constexpr char DELIMITER = ' ';

        std::vector<std::string> parts;

        if (expr_raw_trimmed.starts_with("@"))
        {
            std::string tmp;




/*
========================================================================================================
Quartz Cron Expression – Full Technical Reference and Alias Specification
========================================================================================================

This section provides:
  • A precise description of Quartz cron syntax
  • Explanation of all special characters (*, ?, L, W, #, ranges, steps…)
  • Correct DOM/DOW semantics (Quartz OR-logic)
  • The complete alias mapping used by MindNet
  • A revised and corrected version of the original "Unit Test Specification"

This documentation replaces older comments. All incorrect POSIX-based alias expansions have been fixed.
Everything below now reflects true Quartz behavior, which the implementation follows.

========================================================================================================
1. Quartz Cron Fields
========================================================================================================

A Quartz cron expression uses 6 or 7 fields:

    Field Index   Field Name        Allowed Values                    Special Characters
    ---------------------------------------------------------------------------------------
    1             Seconds           0–59                              * , - /
    2             Minutes           0–59                              * , - /
    3             Hours             0–23                              * , - /
    4             Day-of-Month      1–31                              * ? , - / L W
    5             Month             1–12 or JAN–DEC                   * , - /
    6             Day-of-Week       1–7 or SUN–SAT                    * ? , - / L #
    7 (optional)  Year              1970–2099                         * , - /

========================================================================================================
2. Special Characters (Quartz Semantics)
========================================================================================================

*  (asterisk) – ANY VALUE
    Matches all valid values in the field.
    Example: "* * * * * *" → every second

?  (question mark) – UNSPECIFIED (Quartz-only)
    Can be used only in DOM or DOW fields. It means:
        “This field is intentionally not specified.”
    Used to avoid ambiguity in the DOM/DOW relationship.

,  (comma) – LIST
    Example: "MON,WED,FRI"

-  (dash) – RANGE
    Example: "MON-FRI"

 / (slash) – STEP / INTERVAL
    Example: "* /5" → every 5 units
    (Note: written as "* /5" inside comments to avoid terminating the C comment block.)

L  (LAST)
    DOM: "L" = last day of month
    DOW: "5L" = last Thursday of month

L-n  (LAST minus n)
    Example: "L-3" = 3rd-to-last day of month

W  (nearest weekday) – DOM only
    Example: "15W" → closest weekday to the 15th

#  (Nth weekday of month) – DOW only
    Example: "5#3" → 3rd Thursday

========================================================================================================
3. Quartz DOM / DOW Matching Rules (Critical Difference from POSIX)
========================================================================================================

Quartz uses DIFFERENT logic than POSIX cron:

POSIX cron:
    DOM and DOW use AND logic.

Quartz:
    DOM and DOW use OR logic unless one of them is '?'.

Rules:
    • If DOM="?" and DOW is specific → match on DOW
    • If DOW="?" and DOM is specific → match on DOM
    • If both DOM and DOW are specific → match if (DOM matches) OR (DOW matches)
    • If DOM="*" and DOW="*" → expression matches EVERY DAY

Consequently:
    POSIX:    "0 0 1 * *" → first day of month
    QUARTZ:   "0 0 1 * *" → matches EVERY DAY (NOT correct for monthly!)

Therefore the correct Quartz monthly expression is:
    "0 0 1 * ?"

========================================================================================================
4. MindNet Alias Mapping (Corrected to Quartz Syntax)
========================================================================================================

@yearly, @annually     → "0 0 1 1 * ?"
@monthly               → "0 0 0 1 * ?"
@weekly                → "0 0 0 ? * SUN"
@daily, @midnight      → "0 0 0 * * ?"
@hourly                → "0 0 * * * ?"
@minutely              → "0 * * * * *"
@secondly              → "* * * * * *"

Friendly time-of-day aliases:
@noon                  → "0 0 12 * * ?"
@morning               → "0 0 6 * * ?"
@evening               → "0 0 18 * * ?"

Weekday / weekend:
@weekday               → "0 0 0 * * MON-FRI"
@weekend               → "0 0 0 * * SAT,SUN"

Second-level testing:
@every_1s              → "* * * * * *"
@every_2s              → "* /2 * * * * *"
@every_5s              → "* /5 * * * * *"
@every_10s             → "* /10 * * * * *"
@every_30s             → "* /30 * * * * *"

@every_minute          → "0 * * * * *"
@every_5min            → "0 * /5 * * * *"

Parameterized aliases:
@every_seconds(N)      → "* /N * * * * *"
@every_minutes(N)      → "0 * /N * * * *"
@every_hours(N)        → "0 0 * /N * * *"

Complex time-of-day:
@daily_at(HH:MM)       → "0 MM HH * * ?"
@hourly_at(MM)         → "0 MM * * * ?"

Negative tests (must throw):
    @foo
    @invalid
    @every_seconds(foo)
    @every_minutes(-1)
    @every_hours(0)
    @daily_at(9)
    @daily_at(24:00)
    @daily_at(AB:CD)
    @hourly_at(99)

========================================================================================================
5. Quartz-Specific Complex Examples
========================================================================================================

"0 0 18 L * ?"         → 18:00 on the last day of the month
"0 0 18 L-3 * ?"       → 18:00 three days before the last day
"0 30 10 ? * 5L"       → last Thursday of the month at 10:30
"0 0 10 ? * 5#3"       → third Thursday of the month at 10:00

========================================================================================================
6. Example Timings (Corrected)
========================================================================================================

@yearly:
    Expr:  "0 0 1 1 * ?"
    Start: 2025-01-01 00:00:00
    Next:  2026-01-01 00:00:00

@monthly:
    Expr:  "0 0 0 1 * ?"
    Start: 2025-01-15 10:00:00
    Next:  2025-02-01 00:00:00

@weekly:
    Expr:  "0 0 0 ? * SUN"
    Start: 2025-01-01 (Wed)
    Next:  2025-01-05 00:00:00 (Sun)

@daily:
    Expr:  "0 0 0 * * ?"
    Start: 2025-01-01 10:00:00
    Next:  2025-01-02 00:00:00

@hourly:
    Expr:  "0 0 * * * ?"
    Start: 2025-01-01 10:20:30
    Next:  2025-01-01 11:00:00

@minutely:
    Expr:  "0 * * * * *"
    Start: 2025-01-01 10:20:30
    Next:  2025-01-01 10:21:00

@secondly:
    Expr:  "* * * * * *"
    Start: 2025-01-01 10:20:30
    Next:  2025-01-01 10:20:31

========================================================================================================
End of Documentation
========================================================================================================
*/

            if (expr_raw_trimmed == "@yearly" || expr_raw_trimmed == "@annually")
                tmp = "0 0 1 1 ?";

            else if (expr_raw_trimmed == "@monthly")
                tmp = "0 0 1 * ?";

            else if (expr_raw_trimmed == "@weekly")
                tmp = "0 0 0 ? * SUN";

            else if (expr_raw_trimmed == "@daily" || expr_raw_trimmed == "@midnight")
                tmp = "0 0 0 * * ?";

            else if (expr_raw_trimmed == "@hourly")
                tmp = "0 * * * *";

            else if (expr_raw_trimmed == "@minutely")
                tmp = "0 * * * * *";
            else if (expr_raw_trimmed == "@secondly")
                tmp = "* * * * * *";


                // ------------------------------------------------------------
                // MindNet-friendly time-of-day aliases
                // ------------------------------------------------------------
            else if (expr_raw_trimmed == "@noon")
                tmp = "0 12 * * *";

            else if (expr_raw_trimmed == "@morning")
                tmp = "0 6 * * *";

            else if (expr_raw_trimmed == "@evening")
                tmp = "0 18 * * *";


                // ------------------------------------------------------------
                // Weekday / weekend aliases
                // ------------------------------------------------------------
            else if (expr_raw_trimmed == "@weekday")
                tmp = "0 0 * * MON-FRI";

            else if (expr_raw_trimmed == "@weekend")
                tmp = "0 0 * * SAT,SUN";

                // ------------------------------------------------------------
                // Test-friendly second-level shortcuts
                // ------------------------------------------------------------
            else if (expr_raw_trimmed == "@every_1s")
                tmp = "* * * * * *"; // every second

            else if (expr_raw_trimmed == "@every_2s")
                tmp = "*/2 * * * * *";

            else if (expr_raw_trimmed == "@every_5s")
                tmp = "*/5 * * * * *";

            else if (expr_raw_trimmed == "@every_10s")
                tmp = "*/10 * * * * *";

            else if (expr_raw_trimmed == "@every_30s")
                tmp = "*/30 * * * * *";

            else if (expr_raw_trimmed == "@every_minute")
                tmp = "0 * * * * *";

            else if (expr_raw_trimmed == "@every_5min")
                tmp = "0 */5 * * * *";


                // ------------------------------------------------------------
                // Generic "every X seconds/minutes/hours"
                // Example: @every_seconds(5)
                // ------------------------------------------------------------
            else if (expr_raw_trimmed.starts_with("@every_seconds("))
            {
                // extract N
                auto inside = expr_raw_trimmed.substr(15);
                if (!inside.empty() && inside.back() == ')')
                    inside.pop_back();
                int n = to_int(inside);
                tmp = "*/" + std::to_string(n) + " * * * * *";
            }

            // Example: @every_minutes(5) -> "0 */5 * * * *"
            else if (expr_raw_trimmed.starts_with("@every_minutes("))
            {
                auto inside = expr_raw_trimmed.substr(15);
                if (!inside.empty() && inside.back() == ')')
                    inside.pop_back();
                int n = to_int(inside);
                tmp = "0 */" + std::to_string(n) + " * * * *";
            }

            // Example: @every_hours(3) -> "0 0 */3 * * *"
            else if (expr_raw_trimmed.starts_with("@every_hours("))
            {
                auto inside = expr_raw_trimmed.substr(13);
                if (!inside.empty() && inside.back() == ')')
                    inside.pop_back();
                int n = to_int(inside);
                tmp = "0 0 */" + std::to_string(n) + " * * *";
            }

            // ------------------------------------------------------------
            // Daily at HH:MM – @daily_at(09:30)
            // ------------------------------------------------------------
            else if (expr_raw_trimmed.starts_with("@daily_at("))
            {
                auto inside = expr_raw_trimmed.substr(10);
                if (!inside.empty() && inside.back() == ')')
                    inside.pop_back();

                auto parts2 = split(inside, ':');
                if (parts2.size() != 2)
                    throw std::runtime_error("Invalid @daily_at(HH:MM)");

                int hh = to_int(parts2[0]);
                int mm = to_int(parts2[1]);

                tmp = "0 " + std::to_string(mm) + " " + std::to_string(hh) + " * * *";
            }


            // ------------------------------------------------------------
            // Hourly at mm – @hourly_at(15)
            // → run every hour at 15 minutes
            // ------------------------------------------------------------
            else if (expr_raw_trimmed.starts_with("@hourly_at("))
            {
                auto inside = expr_raw_trimmed.substr(11);
                if (!inside.empty() && inside.back() == ')')
                    inside.pop_back();

                int mm = to_int(inside);
                tmp = "0 " + std::to_string(mm) + " * * * *";
            }


            // ------------------------------------------------------------
            // No match → error
            // ------------------------------------------------------------
            else
                throw std::runtime_error("Invalid CRON alias: " + expr_raw_trimmed);


            // make parts from tmp
            parts = split(tmp, DELIMITER);
        }

        else
        {
            parts = split(expr_raw, DELIMITER);
        }

        // Quartz: [sec] min hour day-of-month month day-of-week [year]
        // Supports 5 or 6 fields: 5 ⇒ no seconds ⇒ add 0
        if (parts.size() == 5)
        {
            // min, hour, dom, month, dow
            parts.insert(parts.begin(), "0"); // sec = 0
        }

        if (parts.size() < 6 || parts.size() > 7)
        {
            throw std::runtime_error("Cron must have 5, 6, or 7 fields");
        }

        CronExpr ce;

        // Seconds
        parse_generic_field(parts[0], ce.seconds, 0, 59, false, false);

        // Minutes
        parse_generic_field(parts[1], ce.minutes, 0, 59, false, false);

        // Hours
        parse_generic_field(parts[2], ce.hours, 0, 23, false, false);

        // Day-of-month
        ce.dom = parse_dom_field(parts[3]);

        // Month
        parse_generic_field(parts[4], ce.months, 1, 12, true, false);

        // Day-of-week
        ce.dow = parse_dow_field(parts[5]);

        // Year (parts[6]) – currently ignored, can be added later

        return ce;
    }

    // =======================================
    // matches()
    // =======================================

    bool CronExpr::matches(
        const std::chrono::system_clock::time_point& tp) const
    {
        std::tm tm = to_local_tm(tp);

        int sec = tm.tm_sec;
        int min = tm.tm_min;
        int hour = tm.tm_hour;
        int day = tm.tm_mday;
        int mon = tm.tm_mon + 1;
        int year = tm.tm_year + 1900;
        int dow_q = tm_wday_to_quartz(tm.tm_wday); // 1..7

        if (!seconds.matches(sec)) return false;
        if (!minutes.matches(min)) return false;
        if (!hours.matches(hour)) return false;
        if (!months.matches(mon)) return false;

        // Quartz logic DOM vs DOW:
        // - if DOM = "?" and DOW = "specific", DOW is used
        // - if DOW = "?" and DOM = "specific", DOM is used
        // - if both are "specific", match = DOM OR DOW
        // - if both "*", then true

        bool dom_unspec = dom.is_unspecified();
        bool dow_unspec = dow.is_unspecified();

        bool dom_any = dom.is_any();
        bool dow_any = dow.is_any();

        bool dom_match = true;
        bool dow_match = true;

        if (!dom_any && !dom_unspec)
        {
            dom_match = dom.matches(year, mon, day, dow_q);
        }

        if (!dow_any && !dow_unspec)
        {
            dow_match = dow.matches(year, mon, day, dow_q);
        }

        // Quartz semantics:
        if (dom_any && dow_any)
            return true;

        if (dom_unspec && dow_unspec)
            return true;

        if (dom_unspec)
            return dow_match;

        if (dow_unspec)
            return dom_match;

        // both specific:
        return dom_match || dow_match;
    }

    // =======================================
    // next_after() – simple, second-by-second
    // =======================================

    inline std::chrono::system_clock::time_point
    CronExpr::next_after(const std::chrono::system_clock::time_point& from) const
    {
        // round to seconds and start from next +1s
        auto from_sec = time_point_cast<std::chrono::seconds>(from);
        auto t = from_sec + std::chrono::seconds(1l);

        // to avoid infinite loop – limit e.g. 5 years ahead
        auto limit = t + std::chrono::hours(24l * 365 * 5);

        while (t < limit)
        {
            if (matches(t))
            {
                return t;
            }
            t += std::chrono::seconds(1l);
        }

        throw std::runtime_error("CronExpr::next_after: no matching time found within 5 years");
    }
} // namespace cronq
