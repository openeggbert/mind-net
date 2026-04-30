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

#include "hive/model/BaseColumns.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/model/ColumnType.hpp"

namespace hive::plugins::repetition::columns
{
    struct R18StateColumns : model::BaseColumns
    {
        R18StateColumns() = delete;

        R18StateColumns(const R18StateColumns&) = delete;
        R18StateColumns& operator=(const R18StateColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r18_state";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* ELIGIBLE = "eligible";
        static constexpr const char* STABILITY_TIMES_100 = "stability_times_100";
        static constexpr const char* LAST_INTERVAL_TIMES_100 = "last_interval_times_100";
        static constexpr const char* REPETITIONS = "repetitions";
        static constexpr const char* LAPSES = "lapses";
        static constexpr const char* NEXT_REVIEW = "next_review";
        static constexpr const char* LAST_REVIEW = "last_review";
        static constexpr const char* LAST_QUALITY = "last_quality";
        static constexpr const char* LAST_SEEN_SEMANTIC_VERSION = "last_seen_semantic_version";
        static constexpr const char* CONTENT_MODIFIED_SINCE_LAST_REVIEW = "content_modified_since_last_review";
    };
}