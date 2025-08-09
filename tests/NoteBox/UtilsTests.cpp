#include <gtest/gtest.h>
#include "NoteBox/Utils.h"
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

namespace NoteBox
{
    TEST(NoteIdToVectorTest, ValidInputs)
    {
        EXPECT_EQ(Utils::note_id_to_vector("1"), vector<string>({"1"}));
        EXPECT_EQ(Utils::note_id_to_vector("12a3b"), vector<string>({"12", "a", "3", "b"}));
        EXPECT_EQ(Utils::note_id_to_vector("123abc"), vector<string>({"123", "abc"}));
        EXPECT_EQ(Utils::note_id_to_vector("1a2b3c"), vector<string>({"1", "a", "2", "b", "3", "c"}));
        EXPECT_EQ(Utils::note_id_to_vector("100x"), vector<string>({"100", "x"}));
        EXPECT_EQ(Utils::note_id_to_vector("42alpha7beta"), vector<string>({"42", "alpha", "7", "beta"}));
    }

    TEST(NoteIdToVectorTest, InvalidEmptyInput)
    {
        EXPECT_THROW({
                     Utils::note_id_to_vector("");
                     }, std::runtime_error);
    }

    TEST(VectorTNoteIdoTest, InvalidStartsWithLetter)
    {
        EXPECT_THROW({
                     Utils::note_id_to_vector("a1");
                     }, std::runtime_error);
    }

    TEST(NoteIdToVectorTest, Inputs)
    {
        EXPECT_EQ(Utils::vector_to_note_id(vector<string>({"1"})), "1");
        EXPECT_EQ(Utils::vector_to_note_id(vector<string>({"12", "a", "3", "b"})), "12a3b");
        EXPECT_EQ(Utils::vector_to_note_id(vector<string>({"123", "abc"})), "123abc");
        EXPECT_EQ(Utils::vector_to_note_id(vector<string>({"1", "a", "2", "b", "3", "c"})), "1a2b3c");
        EXPECT_EQ(Utils::vector_to_note_id(vector<string>({"100", "x"})), "100x");
        EXPECT_EQ(Utils::vector_to_note_id(vector<string>({"42", "alpha", "7", "beta"})), "42alpha7beta");
    }

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

    //
    // next_note_id tests
    //

    TEST(UtilsTest, NextNoteId_RootNoChildren)
    {
        EXPECT_EQ(Utils::next_note_id("", ""), "1");
    }

    TEST(UtilsTest, NextNoteId_RootWithChildren)
    {
        EXPECT_EQ(Utils::next_note_id("", "1"), "2");
        EXPECT_EQ(Utils::next_note_id("", "5"), "6");
    }

    TEST(UtilsTest, NextNoteId_NonRootNoChildren)
    {
        EXPECT_EQ(Utils::next_note_id("1", ""), "1a");
        EXPECT_EQ(Utils::next_note_id("1a", ""), "1a1");
    }

    TEST(UtilsTest, NextNoteId_NonRootWithChildrenDigitIncrement)
    {
        EXPECT_EQ(Utils::next_note_id("1a", "1a1"), "1a2");
    }

    TEST(UtilsTest, NextNoteId_NonRootWithChildrenLetterIncrement)
    {
        EXPECT_EQ(Utils::next_note_id("1", "1a"), "1b");
        EXPECT_EQ(Utils::next_note_id("1", "1z"), "1aa");
    }

////
    struct NextNoteIdTestCase {
        std::string parent_note_id;
        std::string youngest_child_note_id;
        std::string expected_next_id;
    };

    class UtilsNextNoteIdTest : public ::testing::TestWithParam<NextNoteIdTestCase> {};

    TEST_P(UtilsNextNoteIdTest, GeneratesCorrectNextId) {
        const auto& param = GetParam();
        EXPECT_EQ(Utils::next_note_id(param.parent_note_id, param.youngest_child_note_id), param.expected_next_id);
    }

    INSTANTIATE_TEST_SUITE_P(
        NextNoteIdTests,
        UtilsNextNoteIdTest,
        ::testing::Values(
            // Root level, no children → should start at 1
            NextNoteIdTestCase{"", "", "1"},

            // Root level, children exist → increment last numeric id
            NextNoteIdTestCase{"", "1", "2"},
            NextNoteIdTestCase{"", "2", "3"},
            NextNoteIdTestCase{"", "10", "11"},

            // Child notes, no children yet → append 'a' or '1' depending on last char
            NextNoteIdTestCase{"1", "", "1a"},    // parent ends with digit, append 'a'
            NextNoteIdTestCase{"1a", "", "1a1"},  // parent ends with letter, append '1'
            NextNoteIdTestCase{"2b", "", "2b1"},  // parent ends with letter, append '1'
            NextNoteIdTestCase{"10", "", "10a"},  // parent ends with digit, append 'a'

            // Child notes, with existing children → increment last part
            NextNoteIdTestCase{"1a", "1a1", "1a2"},   // last part digit increment
            NextNoteIdTestCase{"1a", "1a2", "1a3"},
            NextNoteIdTestCase{"1a", "1az", "1ba"},   // last part letters increment (z+1 → ba)
            NextNoteIdTestCase{"1b", "1bz", "1ca"},   // letter increment with carry
            NextNoteIdTestCase{"1", "1z", "1aa"},       // edge case? Actually, depends on your note_id_to_vector impl

            // Longer examples
            NextNoteIdTestCase{"1a1", "1a1z", "1a1aa"} // letter increment in last part
        )
    );
    ////
}
