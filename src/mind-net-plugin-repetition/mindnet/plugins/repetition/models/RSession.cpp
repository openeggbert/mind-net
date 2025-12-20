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

#include "mindnet/plugins/repetition/models/RSession.hpp"

namespace mindnet::plugins::repetition::models
{
    create_model_cpp_methods(RSession)

    string RSession::validate()
    {
        using columns::RSessionColumns;

        validator_chain_vector list{
            [this]
            {
                return test_true((map_id > 0) != (map_collection_id > 0),
                                 "either map_id or map_collection_id must be set");
            },
            [this] { return test_true(filter_date_from == 0, "filter_date_from is not yet supported"); },
            [this] { return test_true(filter_date_to == 0, "filter_date_to is not yet supported"); },
            [this] { return test_true(filter_tag == 0, "filter_tag is not yet supported"); },
            [this] { return test_true(filter_collection == 0, "filter_collection is not yet supported"); },
        };
        return util::ValidatorChain::run(list);
    }
}