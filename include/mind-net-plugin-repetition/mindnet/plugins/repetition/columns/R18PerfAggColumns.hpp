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
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ColumnType.hpp"

namespace mindnet::plugins::repetition::columns
{
    struct R18PerfAggColumns : model::BaseColumns
    {
        R18PerfAggColumns() = delete;

        R18PerfAggColumns(const R18PerfAggColumns&) = delete;
        R18PerfAggColumns& operator=(const R18PerfAggColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r18_perf_agg";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* BIN_LOG_T_TIMES_100 = "bin_log_t_times_100";
        static constexpr const char* TOTAL = "total";
        static constexpr const char* CORRECT = "correct";
    };
}