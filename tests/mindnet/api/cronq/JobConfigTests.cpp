#include <gtest/gtest.h>

#include "../../../include/mind-net-api/mindnet/api/cronq/JobConfig.hpp"

namespace mindnet::api::cronq
{
    // ------------------------------------------------------------
    // Helper: create config easily
    // ------------------------------------------------------------

    TEST(JobConfigTest, EmptyConfig)
    {
        JobConfig cfg("");

        auto s = cfg.get_string("x");
        EXPECT_FALSE(s.first.has_value());
        EXPECT_TRUE(s.second.empty());

        auto i = cfg.get_int("x");
        EXPECT_FALSE(i.first.has_value());
        EXPECT_TRUE(i.second.empty());

        auto b = cfg.get_bool("x");
        EXPECT_FALSE(b.first.has_value());
        EXPECT_TRUE(b.second.empty());

        auto s2 = cfg.get_string_or_default("x", "AAA");
        EXPECT_EQ(s2.first, "AAA");
        EXPECT_TRUE(s2.second.empty());

        auto i2 = cfg.get_int_or_default("x", 55);
        EXPECT_EQ(i2.first, 55);
        EXPECT_TRUE(i2.second.empty());

        auto b2 = cfg.get_bool_or_default("x", true);
        EXPECT_EQ(b2.first, true);
        EXPECT_TRUE(b2.second.empty());
    }

    // ------------------------------------------------------------
    // Basic parsing
    // ------------------------------------------------------------

    TEST(JobConfigTest, BasicParsing)
    {
        JobConfig cfg("a=10::b=hello::c=yes");

        auto a = cfg.get_int("a");
        ASSERT_TRUE(a.first.has_value());
        EXPECT_EQ(a.first.value(), 10);
        EXPECT_TRUE(a.second.empty());

        auto b = cfg.get_string("b");
        ASSERT_TRUE(b.first.has_value());
        EXPECT_EQ(b.first.value(), "hello");
        EXPECT_TRUE(b.second.empty());

        auto c = cfg.get_bool("c");
        ASSERT_TRUE(c.first.has_value());
        EXPECT_TRUE(c.first.value());
        EXPECT_TRUE(c.second.empty());
    }

    // ------------------------------------------------------------
    // Parsing integer error
    // ------------------------------------------------------------

    TEST(JobConfigTest, IntParsingError)
    {
        JobConfig cfg("x=abc");

        auto x = cfg.get_int("x");
        EXPECT_FALSE(x.first.has_value());
        EXPECT_EQ(x.second, "Parsing integer failed: abc");

        auto xd = cfg.get_int_or_default("x", 42);
        EXPECT_EQ(xd.first, 42);
        EXPECT_EQ(xd.second, "Parsing integer failed: abc");
    }

    // ------------------------------------------------------------
    // Bool parsing
    // ------------------------------------------------------------

    TEST(JobConfigTest, BoolParsing)
    {
        JobConfig cfg("t=true::f=no::one=1::zero=0::bad=maybe");

        auto t = cfg.get_bool("t");
        ASSERT_TRUE(t.first.has_value());
        EXPECT_TRUE(t.first.value());

        auto f = cfg.get_bool("f");
        ASSERT_TRUE(f.first.has_value());
        EXPECT_FALSE(f.first.value());

        auto one = cfg.get_bool("one");
        ASSERT_TRUE(one.first.has_value());
        EXPECT_TRUE(one.first.value());

        auto zero = cfg.get_bool("zero");
        ASSERT_TRUE(zero.first.has_value());
        EXPECT_FALSE(zero.first.value());

        auto bad = cfg.get_bool("bad");
        EXPECT_FALSE(bad.first.has_value());
        EXPECT_EQ(bad.second, "Parsing boolean failed: maybe");

        auto badDef = cfg.get_bool_or_default("bad", true);
        EXPECT_TRUE(badDef.first);
        EXPECT_EQ(badDef.second, "Parsing boolean failed: maybe");
    }

    // ------------------------------------------------------------
    // String default
    // ------------------------------------------------------------

    TEST(JobConfigTest, StringDefaults)
    {
        JobConfig cfg("x=hello");

        auto ok = cfg.get_string_or_default("x", "default");
        EXPECT_EQ(ok.first, "hello");
        EXPECT_TRUE(ok.second.empty());

        auto none = cfg.get_string_or_default("y", "default");
        EXPECT_EQ(none.first, "default");
        EXPECT_TRUE(none.second.empty());
    }

    // ------------------------------------------------------------
    // Multiple values, missing equal sign, ignored parts
    // ------------------------------------------------------------

    TEST(JobConfigTest, RobustParsing)
    {
        JobConfig cfg("a=1::b=2::broken::c=3::d=");

        auto a = cfg.get_int("a");
        EXPECT_EQ(a.first.value(), 1);

        auto b = cfg.get_int("b");
        EXPECT_EQ(b.first.value(), 2);

        auto c = cfg.get_int("c");
        EXPECT_EQ(c.first.value(), 3);

        auto d = cfg.get_string("d");
        ASSERT_TRUE(d.first.has_value());
        EXPECT_EQ(d.first.value(), "");

        // "broken" part must be ignored completely (no key "broken")
        auto broken = cfg.get_string("broken");
        EXPECT_FALSE(broken.first.has_value());
        EXPECT_TRUE(broken.second.empty());
    }

    // ------------------------------------------------------------
    // Test whitespace handling (optional depending on parsing rules)
    // ------------------------------------------------------------

    TEST(JobConfigTest, WhitespaceHandling)
    {
        JobConfig cfg(" key = value :: x = 10 ");

        auto k = cfg.get_string("key");
        ASSERT_TRUE(k.first.has_value());
        EXPECT_EQ(k.first.value(), "value");

        auto x = cfg.get_int("x");
        ASSERT_TRUE(x.first.has_value());
        EXPECT_EQ(x.first.value(), 10);
    }

    // ------------------------------------------------------------
    // Mixed-case boolean parsing
    // ------------------------------------------------------------

    TEST(JobConfigTest, BoolMixedCase)
    {
        JobConfig cfg("a=TrUe::b=FaLsE::c=YeS::d=Off");

        EXPECT_TRUE(cfg.get_bool("a").first.value());
        EXPECT_FALSE(cfg.get_bool("b").first.value());
        EXPECT_TRUE(cfg.get_bool("c").first.value());
        EXPECT_FALSE(cfg.get_bool("d").first.value());
    }
}
