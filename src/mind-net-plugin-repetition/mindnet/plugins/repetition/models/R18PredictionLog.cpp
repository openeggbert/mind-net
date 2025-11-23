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


#include "mindnet/plugins/repetition/models/R18PredictionLog.hpp"


namespace mindnet::plugins::repetition::models
{
    entity_fields R18PredictionLog::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(predicted_R_times_100);
        result.push_back(actual_grade);
        result.push_back(was_correct);
        return result;
    }

    void R18PredictionLog::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        predicted_R_times_100 = number();
        actual_grade = number();
        was_correct = boolean();
    };

    string R18PredictionLog::validate()
    {
        using columns::R18PredictionLogColumns;

        validator_chain_vector list{
            [this] { return test_not_null(user_id, R18PredictionLogColumns::USER_ID); },
            [this] { return test_at_least(predicted_R_times_100, 0, R18PredictionLogColumns::PREDICTED_R_TIMES_100); },
            [this] { return test_at_least(actual_grade, 0, R18PredictionLogColumns::ACTUAL_GRADE); }
        };
        return util::ValidatorChain::run(list);
    }
}
