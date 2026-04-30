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

#include "hive/plugins/core/models/History.hpp"

namespace hive::plugins::core::models
{
    create_model_cpp_methods(History)
    string History::validate()
    {
        using columns::HistoryColumns;

        validator_chain_vector list{
            [this] { return test_at_least(user_id, 0, HistoryColumns::USER_ID); },
            [this] { return testt_not_empty(table_name, HistoryColumns::TABLE_NAME); },
            [this]
            {
                return test_true(operation == hive::essential::Crudl::List ? true : record_id != 0,
                                 "Record ID must not be 0");
            },
            [this] { return test_ne(cast64(operation), 0, HistoryColumns::OPERATION); },
            [this]
            {
                return test_true(operation == hive::essential::Crudl::List ? true : !data_json.empty(),
                                 "data_json must not be empty");
            },
        };
        return util::ValidatorChain::run(list);
    }
}