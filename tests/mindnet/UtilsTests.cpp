#include <gtest/gtest.h>
#include "mindnet/Utils.h"
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

namespace mindnet
{


    //
    // Tests
    //

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
        EXPECT_EQ(Utils::decimalToLetter(0), 'a');
        EXPECT_EQ(Utils::decimalToLetter(25), 'z');
        EXPECT_EQ(Utils::decimalToLetter(2), 'c');
    }

    TEST(UtilsTest, DecimalToLetter_Invalid)
    {
        EXPECT_THROW(Utils::decimalToLetter(-1), std::out_of_range);
        EXPECT_THROW(Utils::decimalToLetter(26), std::out_of_range);
    }

    TEST(UtilsTest, Base26ToDecimal_Valid)
    {
        EXPECT_EQ(Utils::base26ToDecimal("a"), 0);
        EXPECT_EQ(Utils::base26ToDecimal("z"), 25);
        EXPECT_EQ(Utils::base26ToDecimal("aa"), 26);
        EXPECT_EQ(Utils::base26ToDecimal("ab"), 27);
        EXPECT_EQ(Utils::base26ToDecimal("ba"), 52);
    }

    TEST(UtilsTest, Base26ToDecimal_Invalid)
    {
        EXPECT_THROW(Utils::base26ToDecimal(""), std::invalid_argument);
        EXPECT_THROW(Utils::base26ToDecimal("A"), std::invalid_argument);
        EXPECT_THROW(Utils::base26ToDecimal("a1"), std::invalid_argument);
    }

    TEST(UtilsTest, DecimalToBase26_Valid)
    {
        EXPECT_EQ(Utils::decimalToBase26(0), "a");
        EXPECT_EQ(Utils::decimalToBase26(25), "z");
        EXPECT_EQ(Utils::decimalToBase26(26), "aa");
        EXPECT_EQ(Utils::decimalToBase26(27), "ab");
        EXPECT_EQ(Utils::decimalToBase26(52), "ba");
    }

    TEST(UtilsTest, DecimalToBase26_Invalid)
    {
        EXPECT_THROW(Utils::decimalToBase26(-1), std::invalid_argument);
    }

    ////
}
