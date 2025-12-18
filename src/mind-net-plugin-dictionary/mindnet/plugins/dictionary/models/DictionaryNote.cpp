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

#include "mindnet/plugins/dictionary/models/DictionaryNote.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields DictionaryNote::to_values() const
    {
        return serialize_fields(*this);
    }

    void DictionaryNote::from_values(const entity_fields& values)
    {
        deserialize_fields(*this, values);
    }

    string DictionaryNote::validate()
    {
        using columns::DictionaryNoteColumns;

        validator_chain_vector list{
            [this] { return test_ne(dictionary_term_id, 0, DictionaryNoteColumns::DICTIONARY_TERM_ID); },
            [this] { return testt_between(title, 1, 256, DictionaryNoteColumns::TITLE); },
            [this] { return testt_at_most(content, 10240, DictionaryNoteColumns::CONTENT); },
        };
        return util::ValidatorChain::run(list);
    }
}