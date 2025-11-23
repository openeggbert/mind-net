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


#include "mindnet/plugins/slipbox/models/Note.hpp"


namespace mindnet::plugins::slipbox::models
{
    entity_fields Note::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_id);
        result.push_back(parent_note_id);
        result.push_back(content_id);
        result.push_back(source_id);
        result.push_back(alias_for_note_id);
        result.push_back(title);
        result.push_back(hint);
        result.push_back(sibling_order);
        result.push_back(cast64(importance));
        result.push_back(cast64(difficulty));
        result.push_back(path);
        result.push_back(cast64(depth));

        return result;
    }

    void Note::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_id = number();
        parent_note_id = number();
        content_id = number();
        source_id = number();
        alias_for_note_id = number();
        title = text();
        hint = text();
        sibling_order = number();
        importance = static_cast<enums::Importance>(number());
        difficulty = static_cast<enums::Difficulty>(number());
        path = text();
        depth = number();
    }

    string Note::validate()
    {
        using columns::NoteColumns;

        validator_chain_vector list{
            [this] { return id == 0 ? test_ok() : test_ne(id, parent_note_id, NoteColumns::ID); },
            [this] { return test_ne(map_id, 0, NoteColumns::MAP_ID); },
            [this] { return testt_between(title, 1, 256, NoteColumns::TITLE); },
            [this] { return testt_at_most(hint, 128, NoteColumns::HINT); },
        };
        return util::ValidatorChain::run(list);
    }
}
