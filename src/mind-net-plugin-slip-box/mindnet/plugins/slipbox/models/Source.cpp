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

#include "mindnet/plugins/slipbox/models/Source.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Source::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(cast64(map_id));
        result.push_back(title);
        result.push_back(author);
        result.push_back(cast64(year));
        result.push_back(publisher);
        result.push_back(edition);
        result.push_back(pages);
        result.push_back(url);
        result.push_back(cast64(type));

        return result;
    }

    void Source::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        map_id = number();
        title = text();
        author = text();
        year = number();
        publisher = text();
        edition = text();
        pages = text();
        url = text();
        type = static_cast<enums::SourceType>(number());
    }

    string Source::validate()
    {
        using columns::SourceColumns;

        validator_chain_vector list{
            [this] { return test_ne(map_id, 0, SourceColumns::MAP_ID); },
            [this] { return testt_between(title, 0, 128, SourceColumns::TITLE); },
            [this] { return testt_between(author, 0, 64, SourceColumns::AUTHOR); },
            [this] { return test_at_most(year, 3000, SourceColumns::YEAR); },
            [this] { return testt_between(publisher, 0, 128, SourceColumns::PUBLISHER); },
            [this] { return testt_between(edition, 0, 64, SourceColumns::EDITION); },
            [this] { return testt_between(pages, 0, 64, SourceColumns::PAGES); },
            [this] { return testt_between(url, 0, 256, SourceColumns::URL); },

        };
        return util::ValidatorChain::run(list);
    }
}
