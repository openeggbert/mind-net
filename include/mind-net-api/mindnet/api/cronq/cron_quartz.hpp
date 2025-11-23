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
#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <stdexcept>


// =======================================
// Helpers
// =======================================

namespace mindnet::api::cronq
{
    // =======================================
    // Structures for cron expression fields
    // =======================================

    struct GenericField
    {
        bool any = false; // "*"
        bool has_values = false; // if we have an explicit set
        // Values are indexed from min_value to max_value; index = value - offset
        int min_value = 0;
        int max_value = 0;
        int offset = 0; // typically = min_value
        std::vector<bool> allowed; // allowed[value - offset] == true

        GenericField() = default;

        GenericField(int minv, int maxv)
            : any(false), has_values(false),
              min_value(minv), max_value(maxv),
              offset(minv), allowed(size_t(maxv - minv + 1), false)
        {
        }

        bool matches(int v) const
        {
            if (any)
                return true;

            // If no explicit values were added, treat field as "ANY".
            // This is required because Quartz & cron use special flags
            // (like '?' or complex DOM/DOW logic) that intentionally
            // leave the field without explicit values.
            if (!has_values)
                return true;

            if (v < min_value || v > max_value)
                return false;

            return allowed[size_t(v - offset)];
        }

        void add_value(int v)
        {
            if (v < min_value || v > max_value)
                throw std::runtime_error("Value out of range in GenericField");
            has_values = true;
            allowed[size_t(v - offset)] = true;
        }
    };

    // Day-of-month (DOM)
    struct DayOfMonthField
    {
        enum class Mode
        {
            ANY, // "*"
            UNSPECIFIED, // "?"
            VALUES, // classic values / ranges / steps: "1", "1,10-15/2", ...
            LAST_DAY, // "L"
            LAST_DAY_OFFSET, // "L-n"
            NEAREST_WEEKDAY // "xW"
        };

        Mode mode = Mode::ANY;

        GenericField values; // used when mode == VALUES

        int offset = 0; // for LAST_DAY_OFFSET: L-n
        int weekday_base = 0; // for NEAREST_WEEKDAY: xW → x

        DayOfMonthField() : values(1, 31)
        {
        }

        bool is_any() const { return mode == Mode::ANY; }
        bool is_unspecified() const { return mode == Mode::UNSPECIFIED; }

        // evaluation against a specific date
        bool matches(int year, int month, int day, int dow_quartz) const;
    };

    // Day-of-week (DOW)
    struct DayOfWeekField
    {
        enum class Mode
        {
            ANY,
            UNSPECIFIED,
            VALUES, // list / range / steps: "MON", "MON-FRI", "2,4"
            LAST_WEEKDAY, // "5L" => last Thursday in month (5=THU)
            NTH_WEEKDAY // "5#3" => third Thursday in month
        };

        Mode mode = Mode::ANY;

        GenericField values; // 1..7 for SUN..SAT
        int weekday = 0; // for LAST_WEEKDAY, NTH_WEEKDAY (1..7)
        int nth = 0; // for NTH_WEEKDAY (1..5)

        DayOfWeekField() : values(1, 7)
        {
        }

        bool is_any() const { return mode == Mode::ANY; }
        bool is_unspecified() const { return mode == Mode::UNSPECIFIED; }

        bool matches_simple(int dow_quartz) const
        {
            if (mode == Mode::ANY) return true;
            if (mode == Mode::UNSPECIFIED) return true;
            if (mode == Mode::VALUES) return values.matches(dow_quartz);
            return false;
        }

        bool matches(int year, int month, int day, int dow_quartz) const;
    };

    // Full expression (without year field; can be added later)
    struct CronExpr
    {
        GenericField seconds; // 0-59
        GenericField minutes; // 0-59
        GenericField hours; // 0-23
        DayOfMonthField dom; // 1-31, L, L-n, W, ?, ...
        GenericField months; // 1-12 or JAN-DEC
        DayOfWeekField dow; // 1-7, MON-FRI, 5L, 5#3, ?, ...
        bool run_at_start;

        CronExpr()
            : seconds(0, 59),
              minutes(0, 59),
              hours(0, 23),
              dom(),
              months(1, 12),
              dow(),
              run_at_start(false)
        {
        }

        bool matches(const std::chrono::system_clock::time_point& tp) const;

        // naive but simple resolver – moves second by second
        std::chrono::system_clock::time_point
        next_after(const std::chrono::system_clock::time_point& from) const;
    };

    // =======================================
    // parse_cron_quartz
    // =======================================

    CronExpr parse_cron_quartz(const std::string& expr_raw);

    // =======================================
    // matches()
    // =======================================

    // =======================================
    // next_after() – simple, second-by-second
    // =======================================
} // namespace cronq
