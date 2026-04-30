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

//

#include "hive/plugins/slipbox/models/TestAttemptAnswer.hpp"


namespace hive::plugins::slipbox::models
{
    create_model_cpp_methods(TestAttemptAnswer)

    string TestAttemptAnswer::validate()
    {
        using columns::TestAttemptAnswerColumns;

        validator_chain_vector list{
            [this] { return test_ne(test_attempt_id, 0, TestAttemptAnswerColumns::TEST_ATTEMPT_ID); },
            [this] { return test_ne(question_id, 0, TestAttemptAnswerColumns::QUESTION_ID); },
            [this] { return testt_not_empty(user_answer, TestAttemptAnswerColumns::USER_ANSWER); }
        };

        return util::ValidatorChain::run(list);
    }
}