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
#include <gtest/gtest.h>
#include "mindnet/api/cronq/cron_quartz.hpp"


using namespace mindnet::api::cronq;
using namespace std::chrono;

// -----------------------------------------------------------------------------
// Helper: create system_clock::time_point for given Y-M-D H:M:S
// -----------------------------------------------------------------------------
system_clock::time_point tp(int Y, int M, int D, int h, int m, int s)
{
    std::tm tm{};
    tm.tm_year = Y - 1900;
    tm.tm_mon  = M - 1;
    tm.tm_mday = D;
    tm.tm_hour = h;
    tm.tm_min  = m;
    tm.tm_sec  = s;
    return system_clock::from_time_t(std::mktime(&tm));
}

// -----------------------------------------------------------------------------
// 1. BASIC PARSING TESTS
// -----------------------------------------------------------------------------

TEST(CronQuartz, ParseBasic_SecondsMinutesHours)
{
    auto ce = parse_cron_quartz("*/5 10 3 * * ?");
    EXPECT_TRUE(ce.seconds.matches(0));
    EXPECT_TRUE(ce.seconds.matches(5));
    EXPECT_TRUE(ce.seconds.matches(55));

    EXPECT_TRUE(ce.minutes.matches(10));
    EXPECT_FALSE(ce.minutes.matches(11));

    EXPECT_TRUE(ce.hours.matches(3));
    EXPECT_FALSE(ce.hours.matches(4));
}

TEST(CronQuartz, Parse_MonthNames)
{
    auto ce = parse_cron_quartz("0 0 0 1 JAN,JUN,DEC ?");
    EXPECT_TRUE(ce.months.matches(1));
    EXPECT_TRUE(ce.months.matches(6));
    EXPECT_TRUE(ce.months.matches(12));
    EXPECT_FALSE(ce.months.matches(7));
}

TEST(CronQuartz, Parse_DOW_Names)
{
    auto ce = parse_cron_quartz("0 0 0 ? * MON-FRI");
    EXPECT_TRUE(ce.dow.matches(2025, 1, 1, 4)); // Wednesday = 4
    EXPECT_FALSE(ce.dow.matches(2025, 1, 4, 7)); // Saturday
}

TEST(CronQuartz, Parse_Invalid)
{
    EXPECT_THROW(parse_cron_quartz("@unknown"), std::runtime_error);
    EXPECT_THROW(parse_cron_quartz("0-100 0 0 * * ?"), std::runtime_error);
    EXPECT_THROW(parse_cron_quartz("0 * * *"), std::runtime_error); // too few fields
}

// -----------------------------------------------------------------------------
// 2. QUARTZ DOM/DOW OR LOGIC
// -----------------------------------------------------------------------------

TEST(CronQuartz, Quartz_OR_DOM_DOW)
{
    // "0 0 0 1 * MON" means:
    // match if (day == 1) OR (dow == MON)
    auto ce = parse_cron_quartz("0 0 0 1 * MON");

    // 1st day of month
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 0, 0)));

    // Next Monday (6.1.2025)
    EXPECT_TRUE(ce.matches(tp(2025, 1, 6, 0, 0, 0)));

    // A day that is not 1st AND not Monday
    EXPECT_FALSE(ce.matches(tp(2025, 1, 2, 0, 0, 0)));
}

TEST(CronQuartz, Quartz_DOM_STAR_DOW_STAR_isDaily)
{
    // "* * * * * *" → daily match
    auto ce = parse_cron_quartz("* * * * * *");
    EXPECT_TRUE(ce.matches(tp(2025, 1, 10, 12, 30, 45)));
}

TEST(CronQuartz, Quartz_DOM_unspecified)
{
    // "? * MON" → use only DOW
    auto ce = parse_cron_quartz("0 0 0 ? * MON");
    EXPECT_TRUE(ce.matches(tp(2025, 1, 6, 0, 0, 0))); // Monday
    EXPECT_FALSE(ce.matches(tp(2025, 1, 7, 0, 0, 0))); // Tuesday
}

// -----------------------------------------------------------------------------
// 3. SPECIAL DOM FEATURES: L, L-n, W
// -----------------------------------------------------------------------------

TEST(CronQuartz, DOM_LastDay_L)
{
    auto ce = parse_cron_quartz("0 0 0 L * ?");
    EXPECT_TRUE(ce.matches(tp(2025, 1, 31, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 30, 0, 0, 0)));
}

TEST(CronQuartz, DOM_LastDayOffset_Lminus)
{
    auto ce = parse_cron_quartz("0 0 0 L-3 * ?");
    // January 2025: last day = 31 → 31-3 = 28
    EXPECT_TRUE(ce.matches(tp(2025, 1, 28, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 29, 0, 0, 0)));
}

TEST(CronQuartz, DOM_NearestWeekday_W)
{
    auto ce = parse_cron_quartz("0 0 0 15W * ?");
    // 15 Jan 2025 = Wednesday → matches 15
    EXPECT_TRUE(ce.matches(tp(2025, 1, 15, 0, 0, 0)));

    // If target falls on weekend, run on nearest weekday:
    // 15 June 2024 = Saturday → should run on Friday 14


    std::cout << "---- DEBUG DOM_NearestWeekday_W ----\n";
    std::cout << "seconds.any=" << ce.seconds.any << " has_values=" << ce.seconds.has_values << "\n";
    std::cout << "minutes.any=" << ce.minutes.any << " has_values=" << ce.minutes.has_values << "\n";
    std::cout << "hours.any=" << ce.hours.any << " has_values=" << ce.hours.has_values << "\n";

    std::cout << "DOM mode=" << (int)ce.dom.mode << "\n";
    std::cout << "DOM weekday_base=" << ce.dom.weekday_base << "\n";
    std::cout << "DOM offset=" << ce.dom.offset << "\n";
    std::cout << "DOM.values.has_values=" << ce.dom.values.has_values << "\n";
    std::cout << "DOM.is_any()=" << ce.dom.is_any() << "\n";
    std::cout << "DOM.is_unspecified()=" << ce.dom.is_unspecified() << "\n";

    std::cout << "months.any=" << ce.months.any << " has_values=" << ce.months.has_values << "\n";

    std::cout << "DOW mode=" << (int)ce.dow.mode << "\n";
    std::cout << "DOW.values.has_values=" << ce.dow.values.has_values << "\n";
    std::cout << "DOW.is_any()=" << ce.dow.is_any() << "\n";
    std::cout << "DOW.is_unspecified()=" << ce.dow.is_unspecified() << "\n";

    EXPECT_TRUE(ce.matches(tp(2024, 6, 14, 0, 0, 0)));
}

// -----------------------------------------------------------------------------
// 4. SPECIAL DOW FEATURES: 5L, 5#3
// -----------------------------------------------------------------------------

TEST(CronQuartz, DOW_LastWeekday_5L)
{
    auto ce = parse_cron_quartz("0 0 0 ? * 5L"); // last Thursday

    // January 2025 last Thursday = 30.1.2025
    EXPECT_TRUE(ce.matches(tp(2025, 1, 30, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 23, 0, 0, 0)));
}

TEST(CronQuartz, DOW_NthWeekday_5sharp3)
{
    auto ce = parse_cron_quartz("0 0 0 ? * 5#3"); // 3rd Thursday

    // January 2025: Thursdays = 2,9,16,23,30 → 16 is #3
    EXPECT_TRUE(ce.matches(tp(2025, 1, 16, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 23, 0, 0, 0)));
}

// -----------------------------------------------------------------------------
// 5. ALIASES (MindNet)
// -----------------------------------------------------------------------------

TEST(CronQuartz, Alias_monthly)
{
    auto ce = parse_cron_quartz("@monthly"); // "0 0 0 1 * ?"
    EXPECT_TRUE(ce.matches(tp(2025, 2, 1, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 2, 2, 0, 0, 0)));
}

TEST(CronQuartz, Alias_weekly)
{
    auto ce = parse_cron_quartz("@weekly"); // Sunday
    EXPECT_TRUE(ce.matches(tp(2025, 1, 5, 0, 0, 0))); // Sunday
    EXPECT_FALSE(ce.matches(tp(2025, 1, 6, 0, 0, 0))); // Monday
}

TEST(CronQuartz, Alias_daily)
{
    auto ce = parse_cron_quartz("@daily");
    EXPECT_TRUE(ce.matches(tp(2025, 3, 10, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 3, 10, 1, 0, 0)));
}

TEST(CronQuartz, Alias_every_5s)
{
    auto ce = parse_cron_quartz("@every_5s");
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 0, 0)));
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 0, 5)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 0, 0, 1)));
}

// -----------------------------------------------------------------------------
// 6. next_after()
// -----------------------------------------------------------------------------

TEST(CronQuartz, NextAfter_Seconds)
{
    auto ce = parse_cron_quartz("*/10 * * * * *");

    auto from = tp(2025,1,1, 0,0,3); // 3s
    auto nxt  = ce.next_after(from);

    auto expected = tp(2025,1,1, 0,0,10);
    EXPECT_EQ(system_clock::to_time_t(nxt),
              system_clock::to_time_t(expected));
}

TEST(CronQuartz, NextAfter_Daily)
{
    auto ce = parse_cron_quartz("@daily"); // midnight

    auto from = tp(2025,1,1, 10,0,0);
    auto nxt = ce.next_after(from);

    auto expected = tp(2025,1,2, 0,0,0);
    EXPECT_EQ(system_clock::to_time_t(nxt),
              system_clock::to_time_t(expected));
}

TEST(CronQuartz, NextAfter_Monthly)
{
    auto ce = parse_cron_quartz("@monthly"); // first day of month

    auto from = tp(2025,1,15, 12,0,0);
    auto nxt = ce.next_after(from);

    auto expected = tp(2025,2,1, 0,0,0);
    EXPECT_EQ(system_clock::to_time_t(nxt),
              system_clock::to_time_t(expected));
}

TEST(CronQuartz, NextAfter_DOW_5L)
{
    auto ce = parse_cron_quartz("0 0 0 ? * 5L");

    auto from = tp(2025,1,1, 0,0,0);
    auto nxt  = ce.next_after(from);

    // Last Thursday of January 2025 = 30.1.2025
    auto expected = tp(2025,1,30, 0,0,0);
    EXPECT_EQ(system_clock::to_time_t(nxt),
              system_clock::to_time_t(expected));
}

TEST(CronQuartz, NextAfter_NthWeekday)
{
    auto ce = parse_cron_quartz("0 0 0 ? * 5#3");

    auto from = tp(2025,1,1, 0,0,0);
    auto nxt  = ce.next_after(from);

    auto expected = tp(2025,1,16, 0,0,0);
    EXPECT_EQ(system_clock::to_time_t(nxt),
              system_clock::to_time_t(expected));
}












TEST(CronQuartz, ParseBasic_TimeFields)
{
    auto ce = parse_cron_quartz("30 15 10 * * ?");
    // 10:15:30
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 10, 15, 30)));

    // wrong second
    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 10, 15, 29)));

    // wrong minute
    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 10, 14, 30)));

    // wrong hour
    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 9, 15, 30)));
}
TEST(CronQuartz, Parse_Ranges)
{
    auto ce = parse_cron_quartz("0 0 9-17 * * ?");

    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 9, 0, 0)));
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 17, 0, 0)));

    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 8, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 18, 0, 0)));
}
TEST(CronQuartz, Parse_Steps)
{
    auto ce = parse_cron_quartz("0 */10 * * * ?");

    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 0, 0)));
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 10, 0)));
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 20, 0)));

    EXPECT_FALSE(ce.matches(tp(2025, 1, 1, 0, 11, 0)));
}
TEST(CronQuartz, Parse_DOW_Names2)
{
    auto ce = parse_cron_quartz("0 0 0 ? * MON");
    EXPECT_TRUE(ce.matches(tp(2025, 1, 6, 0, 0, 0))); // Monday
    EXPECT_FALSE(ce.matches(tp(2025, 1, 7, 0, 0, 0))); // Tuesday
}
TEST(CronQuartz, Parse_MonthNames2)
{
    auto ce = parse_cron_quartz("0 0 0 1 JAN ?");
    EXPECT_TRUE(ce.matches(tp(2025, 1, 1, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 2, 1, 0, 0, 0)));
}
TEST(CronQuartz, DOM_LastDay)
{
    auto ce = parse_cron_quartz("0 0 0 L * ?");

    EXPECT_TRUE(ce.matches(tp(2025, 1, 31, 0, 0, 0)));
    EXPECT_TRUE(ce.matches(tp(2024, 2, 29, 0, 0, 0))); // leap year

    EXPECT_FALSE(ce.matches(tp(2025, 1, 30, 0, 0, 0)));
}
TEST(CronQuartz, DOM_LastDay_Offset)
{
    auto ce = parse_cron_quartz("0 0 0 L-2 * ?");

    // Jan 2025: 31 → L-2 = 29
    EXPECT_TRUE(ce.matches(tp(2025, 1, 29, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2025, 1, 30, 0, 0, 0)));
}
TEST(CronQuartz, DOW_LastWeekday)
{
    auto ce = parse_cron_quartz("0 0 0 ? * 5L");

    // June 2024: last Friday was 28th
    EXPECT_TRUE(ce.matches(tp(2024, 6, 28, 0, 0, 0)));

    EXPECT_FALSE(ce.matches(tp(2024, 6, 21, 0, 0, 0)));
}
TEST(CronQuartz, DOW_NthWeekday)
{
    auto ce = parse_cron_quartz("0 0 0 ? * 5#3");

    // June 2024: 3rd Friday = 21st
    EXPECT_TRUE(ce.matches(tp(2024, 6, 21, 0, 0, 0)));
    EXPECT_FALSE(ce.matches(tp(2024, 6, 14, 0, 0, 0)));
}
TEST(CronQuartz, DOM_and_DOW_OR_logic)
{
    auto ce = parse_cron_quartz("0 0 0 10 * MON");

    // Match if either 10th day OR Monday
    EXPECT_TRUE(ce.matches(tp(2025, 2, 10, 0, 0, 0))); // 10th
    EXPECT_TRUE(ce.matches(tp(2025, 2, 17, 0, 0, 0))); // Monday
    EXPECT_FALSE(ce.matches(tp(2025, 2, 11, 0, 0, 0))); // neither
}

