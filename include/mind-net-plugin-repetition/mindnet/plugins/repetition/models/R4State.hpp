#pragma once


#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model R4State
#define MODEL R4_STATE
#define COLS columns::R4StateColumns
#include "../columns/R4StateColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R4_STATE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_group("Repetition", 200).set_title_column(COLS::INTERVAL).allow_reader_write()
        .set_rest_operations("rl")
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | READONLY),

            coldef(COLS::REPETITIONS, INTEGER).set_default_value(0),
            coldef(COLS::INTERVAL, INTEGER).set_default_value(1),
            coldef(COLS::EF_TIMES_100, INTEGER).set_default_value(250),
            coldef(COLS::CORRECTION_FACTOR_TIMES_100, INTEGER).set_default_value(100),

            coldef(COLS::NEXT_REVIEW, DATETIME),
            coldef(COLS::LAST_REVIEW, DATETIME),
            coldef(COLS::LAST_QUALITY, INTEGER).set_default_value(0),
            coldef(COLS::LAST_SEEN_SEMANTIC_VERSION, INTEGER).set_default_value(1),
            coldef(COLS::CONTENT_MODIFIED_SINCE_LAST_REVIEW, BOOL).set_default_value(false),
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int note_id{};
        int repetitions{};
        int interval{1};
        int ef_times_100{250};
        int correction_factor_times_100{100};
        unixtime next_review;
        unixtime last_review;
        int last_quality{};
        int last_seen_semantic_version{};
        bool content_modified_since_last_review{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                repetitions == other.repetitions &&
                interval == other.interval &&
                ef_times_100 == other.ef_times_100 &&
                correction_factor_times_100 == other.correction_factor_times_100 &&
                next_review == other.next_review &&
                last_review == other.last_review &&
                last_quality == other.last_quality &&
                last_seen_semantic_version == other.last_seen_semantic_version &&
                content_modified_since_last_review == other.content_modified_since_last_review;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
