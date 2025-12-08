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

#include "mindnet/plugins/dictionary/models/Project.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields Project::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(title);
        result.push_back(description);
        result.push_back(progress);
        result.push_back(cast64(in_progress));
        result.push_back(cast64(important));
        result.push_back(cast64(due_date));
        result.push_back(created_by);
        result.push_back(owner_id);
        result.push_back(assigned_to);
        result.push_back(category);
        result.push_back(cast64(is_public));
        return result;
    }

    void Project::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        note_id = number();
        title = text();
        description = text();
        progress = number();
        in_progress = number();
        important = number();
        due_date = number();
        created_by = number();
        owner_id = number();
        assigned_to = number();
        category = text();
        is_public = number();
    }

    string Project::validate()
    {
        using columns::ProjectColumns;

        validator_chain_vector list{
            [this] { return testt_not_empty(title, ProjectColumns::TITLE); },
            [this] { return test_between(progress, 0, 100, ProjectColumns::PROGRESS); },
            [this] { return test_ne(created_by, 0, ProjectColumns::CREATED_BY); },
            [this] { return test_ne(owner_id, 0, ProjectColumns::OWNER_ID); }
        };
        return util::ValidatorChain::run(list);
    }
}