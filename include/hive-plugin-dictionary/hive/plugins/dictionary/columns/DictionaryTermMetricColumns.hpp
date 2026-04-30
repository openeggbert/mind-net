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

namespace hive::plugins::dictionary::columns
{
    struct DictionaryTermMetricColumns : model::BaseColumns
    {
        DictionaryTermMetricColumns() = delete;

        DictionaryTermMetricColumns(const DictionaryTermMetricColumns&) = delete;
        DictionaryTermMetricColumns& operator=(const DictionaryTermMetricColumns&) = delete;

        static constexpr const char* MODEL_NAME = "dictionary_term_metric";

        static constexpr const char* DICTIONARY_TERM_ID = "dictionary_term_id";
        static constexpr const char* DICTIONARY_MAP_ID = "dictionary_map_id";

        static constexpr const char* TAG_COUNT    = "tag_count";
        static constexpr const char* FLAG_COUNT   = "flag_count";
        static constexpr const char* ALIAS_COUNT  = "alias_count";
        static constexpr const char* NOTE_COUNT   = "note_count";
        static constexpr const char* LINK_COUNT   = "link_count";
        static constexpr const char* BACKLINK_COUNT   = "backlink_count";
        static constexpr const char* URL_COUNT   = "url_count";
        static constexpr const char* SOURCE_COUNT = "source_count";
        static constexpr const char* INDEX_COUNT  = "index_count";
        static constexpr const char* REVIEW_COUNT  = "review_count";
        static constexpr const char* STATE_18_COUNT  = "state_18_count";

        static constexpr const char* UPDATE_COUNT      = "update_count";
        static constexpr const char* VIEW_COUNT      = "view_count";
        static constexpr const char* LAST_VIEWED_AT  = "last_viewed_at";
        static constexpr const char* LAST_UPDATED_AT  = "last_updated_at";
    };
}