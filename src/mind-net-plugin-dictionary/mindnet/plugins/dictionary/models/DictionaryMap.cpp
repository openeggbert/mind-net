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

#include "mindnet/plugins/dictionary/models/DictionaryMap.hpp"

namespace mindnet::plugins::dictionary::models
{
    create_model_cpp_methods(DictionaryMap)

    string DictionaryMap::validate()
    {
        using columns::DictionaryMapColumns;

        validator_chain_vector list{
            [this] { return testt_between(name, 1, 80, DictionaryMapColumns::NAME); },
            [this] { return testt_between(description, 0, 512, DictionaryMapColumns::DESCRIPTION); },
            [this] { return test_ne(owner_id, 0, DictionaryMapColumns::OWNER_ID); },
            [this] { return test_between(cast64(owner_rights), 0, 7, DictionaryMapColumns::OWNER_RIGHTS); },
            [this] { return test_between(cast64(team_rights), 0, 7, DictionaryMapColumns::TEAM_RIGHTS); },
            [this] { return test_between(cast64(other_rights), 0, 7, DictionaryMapColumns::OTHER_RIGHTS); },
        };
        return util::ValidatorChain::run(list);
    }
}