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

#include "mindnet/plugins/repetition/models/R4State.hpp"

namespace mindnet::plugins::repetition::models
{
    entity_fields R4State::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(repetitions);
        result.push_back(interval);
        result.push_back(ef_times_100);
        result.push_back(correction_factor_times_100);
        result.push_back(cast64(next_review));
        result.push_back(cast64(last_review));
        result.push_back(last_quality);
        result.push_back(last_seen_semantic_version);
        result.push_back(content_modified_since_last_review);
        return result;
    }

    void R4State::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        repetitions = number();
        interval = number();
        ef_times_100 = number();
        correction_factor_times_100 = number();
        next_review = number();
        last_review = number();
        last_quality = number();
        last_seen_semantic_version = number();
        content_modified_since_last_review = boolean();
    }

    string R4State::validate()
    {
        using columns::R4StateColumns;

        validator_chain_vector list{
            [this] { return test_at_least(repetitions, 0, R4StateColumns::REPETITIONS); },
            [this] { return test_at_least(interval, 1, R4StateColumns::INTERVAL); },
            [this] { return test_between(ef_times_100, 100, 500, R4StateColumns::EF_TIMES_100); },
            [this]
            {
                return test_between(correction_factor_times_100, 0, 110, R4StateColumns::CORRECTION_FACTOR_TIMES_100);
            },
            [this] { return test_between(last_quality, 0, 5, R4StateColumns::LAST_QUALITY); },
            [this] { return test_at_least(last_seen_semantic_version, 1, R4StateColumns::LAST_SEEN_SEMANTIC_VERSION); },
        };
        return util::ValidatorChain::run(list);
    }
}