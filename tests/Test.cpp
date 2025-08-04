//
// Created by robertvokac on 5/17/25.
//


#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(MathTest, AdditionWorks) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
}

