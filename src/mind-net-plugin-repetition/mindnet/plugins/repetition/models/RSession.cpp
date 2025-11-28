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

#include "mindnet/plugins/repetition/models/RSession.hpp"

namespace mindnet::plugins::repetition::models
{
    entity_fields RSession::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(map_id);
        result.push_back(map_collection_id);
        result.push_back(cloned_from_session_id);
        result.push_back(cast64(algorithm));
        result.push_back(cast64(schedule));
        result.push_back(cast64(scope));
        result.push_back(description);
        result.push_back(filter_under_note);
        result.push_back(cast64(filter_date_from));
        result.push_back(cast64(filter_date_to));
        result.push_back(filter_tag);
        result.push_back(filter_collection);
        result.push_back(selected_items);
        result.push_back(cast64(pinned));
        return result;
    }

    void RSession::from_values(const entity_fields& values)
    {
        int i = 0;

        //todo: check, why there is no macro

        auto number = [&values, &i]
        {
            // std::cout << "values[" << i << "]" << std::endl;
            std::int64_t result = std::get<std::int64_t>(values[i++]);
            // std::cout << "result[" << i << "]" << result << std::endl;
            return result;
        };
        auto boolean = [&number] { return number() != 0; };
        auto text = [&values, &i] { return std::get<std::string>(values[i++]); };

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        map_id = number();
        map_collection_id = number();
        cloned_from_session_id = number();
        algorithm = enums::int_to_repetition_algorithm(number());
        schedule = enums::int_to_repetition_schedule(number());
        scope = enums::int_to_repetition_scope(number());
        description = text();
        filter_under_note = number();
        filter_date_from = number();
        filter_date_to = number();
        filter_tag = number();
        filter_collection = number();
        selected_items = text();
        pinned = boolean();
    };

    string RSession::validate()
    {
        using columns::RSessionColumns;

        validator_chain_vector list{
            [this]
            {
                return test_true((map_id > 0) != (map_collection_id > 0),
                                 "either map_id or map_collection_id must be set"); },
            [this] { return test_true(filter_date_from == 0, "filter_date_from is not yet supported"); },
            [this] { return test_true(filter_date_to == 0, "filter_date_to is not yet supported"); },
            [this] { return test_true(filter_tag == 0, "filter_tag is not yet supported"); },
            [this] { return test_true(filter_collection == 0, "filter_collection is not yet supported"); },
        };
        return util::ValidatorChain::run(list);
    }
}
