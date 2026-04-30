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

#include "hive/plugins/dictionary/models/DictionarySearch.hpp"

namespace hive::plugins::dictionary::models
{
    create_model_cpp_methods(DictionarySearch)

    string DictionarySearch::validate()
    {
        using columns::DictionarySearchColumns;

        validator_chain_vector list{
            // ownership / scope
            [this] { return test_ne(user_id, 0, DictionarySearchColumns::USER_ID); },
            [this] { return test_ne(dictionary_map_id, 0, DictionarySearchColumns::DICTIONARY_MAP_ID); },

            // metadata
            [this] { return testt_not_empty(name, DictionarySearchColumns::NAME); },
            [this] {return util::testt_trimmed(name, DictionarySearchColumns::NAME);},

            // core definition
            [this] { return testt_not_empty(query_json, DictionarySearchColumns::QUERY_JSON); },
        };

        return util::ValidatorChain::run(list);
    }
}
