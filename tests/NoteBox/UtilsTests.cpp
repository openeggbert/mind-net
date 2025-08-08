#include <gtest/gtest.h>
#include "NoteBox/Utils.h"
#include <vector>
#include <string>

using namespace std;

TEST(NoteIdToVectorTest, ValidInputs) {
    EXPECT_EQ(Utils::note_id_to_vector("1"), vector<string>({"1"}));
    EXPECT_EQ(Utils::note_id_to_vector("12a3b"), vector<string>({"12", "a", "3", "b"}));
    EXPECT_EQ(Utils::note_id_to_vector("123abc"), vector<string>({"123", "abc"}));
    EXPECT_EQ(Utils::note_id_to_vector("1a2b3c"), vector<string>({"1", "a", "2", "b", "3", "c"}));
    EXPECT_EQ(Utils::note_id_to_vector("100x"), vector<string>({"100", "x"}));
    EXPECT_EQ(Utils::note_id_to_vector("42alpha7beta"), vector<string>({"42", "alpha", "7", "beta"}));
}

TEST(NoteIdToVectorTest, InvalidEmptyInput) {
    EXPECT_THROW({
        Utils::note_id_to_vector("");
    }, std::runtime_error);
}

TEST(NoteIdToVectorTest, InvalidStartsWithLetter) {
    EXPECT_THROW({
        Utils::note_id_to_vector("a1");
    }, std::runtime_error);
}
