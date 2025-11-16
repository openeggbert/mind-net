#pragma once


#include "mindnet/model/BaseColumns.hpp"

namespace mindnet::plugins::repetition::columns
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
