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

#include "mindnet/plugins/dictionary/models/Idea.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields Idea::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        //
        result.push_back(cast64(user_id));
        result.push_back(title);
        result.push_back(content);
        result.push_back(category);
        result.push_back(cast64(due_at));
        result.push_back(important);
        result.push_back(is_public);
        result.push_back(pinned);
        return result;
    }

    void Idea::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        user_id = number();
        title = text();
        content = text();
        category = text();
        due_at = number();
        important = boolean();
        is_public = boolean();
        pinned = boolean();
    }

    string Idea::validate()
    {
        using columns::IdeaColumns;

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0, IdeaColumns::USER_ID); },
            [this] { return testt_not_empty(title, IdeaColumns::TITLE); },
            [this] { return testt_at_most(content, 1024, IdeaColumns::CONTENT); },
            [this] { return testt_at_most(category, 32, IdeaColumns::CATEGORY); },
            [this] { return test_at_least(due_at, 0, IdeaColumns::DUE_AT); },

        };
        return util::ValidatorChain::run(list);
    }
}