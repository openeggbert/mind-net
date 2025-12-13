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

#include "mindnet/plugins/dictionary/models/DictionarySourceType.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields DictionarySourceType::to_values() const
    {
        return serialize_fields(*this);
    }

    void DictionarySourceType::from_values(const entity_fields& values)
    {
        deserialize_fields(*this, values);
    }

    string DictionarySourceType::validate()
    {
        using columns::DictionarySourceTypeColumns;

        validator_chain_vector list{
            [this] { return testt_between(title, 0, 128, DictionarySourceTypeColumns::TITLE); },
            [this] { return testt_between(author, 0, 64, DictionarySourceTypeColumns::AUTHOR); },
            [this] { return test_at_most(year, 3000, DictionarySourceTypeColumns::YEAR); },
            [this] { return testt_between(publisher, 0, 128, DictionarySourceTypeColumns::PUBLISHER); },
            [this] { return testt_between(edition, 0, 64, DictionarySourceTypeColumns::EDITION); },
            [this] { return testt_between(pages, 0, 64, DictionarySourceTypeColumns::PAGES); },
            [this] { return testt_between(url, 0, 256, DictionarySourceTypeColumns::URL); },

        };
        return util::ValidatorChain::run(list);
    }
}