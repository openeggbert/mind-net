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

#include "mindnet/plugins/dictionary/models/TestAttempt.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields TestAttempt::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));

        result.push_back(test_id);
        result.push_back(user_id);
        result.push_back(attempt_number);
        result.push_back(cast64(started_at));
        result.push_back(cast64(finished_at));
        result.push_back(score_times_100);
        result.push_back(question_ids);

        return result;
    }

    void TestAttempt::from_values(const entity_fields& values)
    {
        int i = 0;
        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();

        test_id = number();
        user_id = number();
        attempt_number = number();
        started_at = number();
        finished_at = number();
        score_times_100 = number();
        question_ids = text();
    }

    string TestAttempt::validate()
    {
        using columns::TestAttemptColumns;

        validator_chain_vector list{
            [this] { return test_ne(test_id, 0, TestAttemptColumns::TEST_ID); },
            [this] { return test_ne(user_id, 0, TestAttemptColumns::USER_ID); },
            [this] { return test_at_least(attempt_number, 1, TestAttemptColumns::ATTEMPT_NUMBER); },
            [this]
            {
                return test_true(finished_at == 0 ? true : finished_at > started_at,
                                 "finished_at must be later than started_at");
            },
            [this] { return test_between(score_times_100, 0, 10000, TestAttemptColumns::SCORE_TIMES_100); },
        };

        return util::ValidatorChain::run(list);
    }
}