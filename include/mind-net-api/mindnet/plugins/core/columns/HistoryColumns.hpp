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


/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#pragma once


#include "mindnet/model/BaseColumns.hpp"


namespace mindnet::plugins::core::columns
{
    struct HistoryColumns : model::BaseColumns
    {
        HistoryColumns() = delete;

        HistoryColumns(const HistoryColumns&) = delete;
        HistoryColumns& operator=(const HistoryColumns&) = delete;

        static constexpr const char* MODEL_NAME = "history";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* TABLE_NAME = "table_name";
        static constexpr const char* RECORD_ID = "record_id";
        static constexpr const char* OPERATION = "operation";
        static constexpr const char* DATA_JSON = "data_json";
        static constexpr const char* REASON = "reason";
    };
}
