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

#include "mindnet/plugins/slipbox/models/Content.hpp"


namespace mindnet::plugins::slipbox::models
{
    entity_fields Content::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(value);
        result.push_back(cast64(format));
        result.push_back(version);
        result.push_back(semantic_version);
        result.push_back(change_ratio);
        result.push_back(cast64(last_parsed_success_at));
        result.push_back(cast64(last_parsed_fail_at));
        return result;
    }

    void Content::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        value = text();
        format = static_cast<enums::ContentFormat>(number());
        version = number();
        semantic_version = number();
        change_ratio = number();
        last_parsed_success_at = number();
        last_parsed_fail_at = number();
    }

    string Content::validate()
    {
        using columns::ContentColumns;

        validator_chain_vector list{
            [this] { return test_between(change_ratio, 0, 100, ContentColumns::CHANGE_RATIO);},
        };
        return util::ValidatorChain::run(list);
    }
}
