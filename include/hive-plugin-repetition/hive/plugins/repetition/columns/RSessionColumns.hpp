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
#pragma once

#include "hive/model/BaseColumns.hpp"

namespace hive::plugins::repetition::columns
{
    struct RSessionColumns : model::BaseColumns
    {
        RSessionColumns() = delete;
        RSessionColumns(const RSessionColumns&) = delete;
        RSessionColumns& operator=(const RSessionColumns&) = delete;

        // Table name
        static constexpr const char* MODEL_NAME = "r_session";

        // Required foreign keys and relationships
        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* MAP_COLLECTION_ID = "map_collection_id";
        static constexpr const char* CLONED_FROM_SESSION_ID = "cloned_from_session_id";

        // Core session settings
        static constexpr const char* ALGORITHM = "algorithm";
        static constexpr const char* SCHEDULE = "schedule";
        static constexpr const char* SCOPE = "scope";
        static constexpr const char* DESCRIPTION = "description";

        // Filter settings
        static constexpr const char* FILTER_ELIGIBLE = "filter_eligible";
        static constexpr const char* FILTER_UNDER_NOTE = "filter_under_note";
        static constexpr const char* FILTER_DATE_FROM = "filter_date_from";
        static constexpr const char* FILTER_DATE_TO = "filter_date_to";
        static constexpr const char* FILTER_TAG = "filter_tag";
        static constexpr const char* FILTER_COLLECTION = "filter_collection";

        // Session state 
        static constexpr const char* SELECTED_ITEMS = "selected_items";
        // JSON format: {"note_ids":[...], "question_ids":[...]}
        static constexpr const char* PINNED = "pinned";
    };
}