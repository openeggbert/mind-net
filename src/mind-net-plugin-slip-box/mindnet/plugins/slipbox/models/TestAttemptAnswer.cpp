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

#include "mindnet/plugins/slipbox/models/TestAttemptAnswer.hpp"


namespace mindnet::plugins::slipbox::models
{
    entity_fields TestAttemptAnswer::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));

        result.push_back(test_attempt_id);
        result.push_back(question_id);
        result.push_back(user_answer);
        result.push_back(is_correct);

        return result;
    }

    void TestAttemptAnswer::from_values(const entity_fields& values)
    {
        int i = 0;
        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();

        test_attempt_id = number();
        question_id = number();
        user_answer = text();
        is_correct = boolean();
    }

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
