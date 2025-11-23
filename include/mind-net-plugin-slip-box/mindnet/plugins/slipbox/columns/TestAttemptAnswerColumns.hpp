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


/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#pragma once


#include "mindnet/model/BaseColumns.hpp"


namespace mindnet::plugins::slipbox::columns
{
    struct TestAttemptAnswerColumns : model::BaseColumns
    {
        TestAttemptAnswerColumns() = delete;

        TestAttemptAnswerColumns(const TestAttemptAnswerColumns&) = delete;
        TestAttemptAnswerColumns& operator=(const TestAttemptAnswerColumns&) = delete;

        static constexpr const char* MODEL_NAME = "test_attempt_answer";

        static constexpr const char* TEST_ATTEMPT_ID = "test_attempt_id";
        static constexpr const char* QUESTION_ID = "question_id";

        static constexpr const char* USER_ANSWER = "user_answer";
        static constexpr const char* IS_CORRECT = "is_correct";
    };
}
