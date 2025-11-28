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

#include "mindnet/plugins/slipbox/models/Link.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Link::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(from_note_id);
        result.push_back(to_note_id);
        result.push_back(to_note_title);
        result.push_back(label);
        return result;
    }

    void Link::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        from_note_id = number();
        to_note_id = number();
        to_note_title = text();
        label = text();
    };

    string Link::validate()
    {
        using columns::LinkColumns;

        validator_chain_vector list{
            [this] { return test_ne(from_note_id, 0, LinkColumns::FROM_NOTE_ID); },
            [this] { return test_ne(to_note_id, 0, LinkColumns::TO_NOTE_ID); },
            [this] { return testt_not_empty(to_note_title, LinkColumns::TO_NOTE_TITLE); },
        };
        return util::ValidatorChain::run(list);
    }
}