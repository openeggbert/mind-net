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
#include "../../include/mind-net-util/mindnet/util/Utils.hpp"
#include <vector>
#include <string>
#include <stdexcept>


using namespace std;

namespace mindnet::util
{
    TEST(UtilsTest, LetterToDecimal_Valid)
    {
        EXPECT_EQ(Utils::letterToDecimal('a'), 0);
        EXPECT_EQ(Utils::letterToDecimal('z'), 25);
        EXPECT_EQ(Utils::letterToDecimal('c'), 2);
    }

    TEST(UtilsTest, LetterToDecimal_Invalid)
    {
        EXPECT_THROW(Utils::letterToDecimal('A'), std::invalid_argument);
        EXPECT_THROW(Utils::letterToDecimal('1'), std::invalid_argument);
        EXPECT_THROW(Utils::letterToDecimal('{'), std::invalid_argument);
    }

    TEST(UtilsTest, DecimalToLetter_Valid)
    {
        EXPECT_EQ(Utils::decimal_to_letter(0), 'a');
        EXPECT_EQ(Utils::decimal_to_letter(25), 'z');
        EXPECT_EQ(Utils::decimal_to_letter(2), 'c');
    }

    TEST(UtilsTest, DecimalToLetter_Invalid)
    {
        EXPECT_THROW(Utils::decimal_to_letter(-1), std::out_of_range);
        EXPECT_THROW(Utils::decimal_to_letter(26), std::out_of_range);
    }

    TEST(UtilsTest, Base26ToDecimal_Valid)
    {
        EXPECT_EQ(Utils::base26_to_decimal("a"), 0);
        EXPECT_EQ(Utils::base26_to_decimal("z"), 25);
        EXPECT_EQ(Utils::base26_to_decimal("aa"), 26);
        EXPECT_EQ(Utils::base26_to_decimal("ab"), 27);
        EXPECT_EQ(Utils::base26_to_decimal("ba"), 52);
    }

    TEST(UtilsTest, Base26ToDecimal_Invalid)
    {
        EXPECT_THROW(Utils::base26_to_decimal(""), std::invalid_argument);
        EXPECT_THROW(Utils::base26_to_decimal("A"), std::invalid_argument);
        EXPECT_THROW(Utils::base26_to_decimal("a1"), std::invalid_argument);
    }

    TEST(UtilsTest, DecimalToBase26_Valid)
    {
        EXPECT_EQ(Utils::decimal_to_base26(0), "a");
        EXPECT_EQ(Utils::decimal_to_base26(25), "z");
        EXPECT_EQ(Utils::decimal_to_base26(26), "aa");
        EXPECT_EQ(Utils::decimal_to_base26(27), "ab");
        EXPECT_EQ(Utils::decimal_to_base26(52), "ba");
    }

    TEST(UtilsTest, DecimalToBase26_Invalid)
    {
        EXPECT_THROW(Utils::decimal_to_base26(-1), std::invalid_argument);
    }

    ////
}
