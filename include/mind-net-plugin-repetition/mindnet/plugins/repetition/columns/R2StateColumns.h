#ifndef R2STATECOLUMNS_H
#define R2STATECOLUMNS_H

#include "mindnet/model/BaseColumns.h"
#include "mindnet/essential/Helper.h"
#include "mindnet/model/ColumnType.h"

namespace mindnet::plugins::repetition::columns
{
    struct R2StateColumns : model::BaseColumns
    {
        R2StateColumns() = delete;

        R2StateColumns(const R2StateColumns&) = delete;
        R2StateColumns& operator=(const R2StateColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r2_state";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* QUESTION_ID = "question_id";
        static constexpr const char* REPETITIONS = "repetitions";
        static constexpr const char* INTERVAL = "interval";
        static constexpr const char* EF_TIMES_100 = "ef_times_100";
        static constexpr const char* NEXT_REVIEW = "next_review";
        static constexpr const char* LAST_REVIEW = "last_review";
        static constexpr const char* LAST_QUALITY = "last_quality";
    };
}
#endif // R2STATECOLUMNS_H
