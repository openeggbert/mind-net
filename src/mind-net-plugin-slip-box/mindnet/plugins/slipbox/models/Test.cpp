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

#include "mindnet/plugins/slipbox/models/Test.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Test::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));

        result.push_back(under_note_id);
        result.push_back(map_id);
        result.push_back(title);
        result.push_back(description);
        result.push_back(time_limit_in_seconds);
        result.push_back(answer_count_limit);
        result.push_back(attempt_limit);
        result.push_back(is_public);

        return result;
    }

    void Test::from_values(const entity_fields& values)
    {
        int i = 0;
        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();

        under_note_id = number();
        map_id = number();
        title = text();
        description = text();
        time_limit_in_seconds = number();
        answer_count_limit = number();
        attempt_limit = number();
        is_public = boolean();
    }

    string Test::validate()
    {
        using columns::TestColumns;

        validator_chain_vector list{
            [this] { return test_ne(under_note_id, 0, TestColumns::UNDER_NOTE_ID); },
            [this] { return test_ne(map_id, 0, TestColumns::MAP_ID); },
            [this] { return testt_between(title, 3, 256, TestColumns::TITLE); },
            [this] { return test_at_least(time_limit_in_seconds, 60, TestColumns::ANSWER_COUNT_LIMIT); },
            [this] { return test_at_least(answer_count_limit, 1, TestColumns::ANSWER_COUNT_LIMIT); },
            [this] { return test_at_least(attempt_limit, 1, TestColumns::ATTEMPT_LIMIT); }
        };

        return util::ValidatorChain::run(list);
    }
}