#ifndef RREVIEW_H
#define RREVIEW_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model RReview
#define MODEL R_REVIEW
#define COLS columns::RReviewColumns
#include "../columns/RReviewColumns.h"
#include "mindnet/plugins/repetition/enums/RepetitionAlgorithm.h"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def R_REVIEW_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Repetition", 200).set_title_column(COLS::NOTE_ID).allow_reader_write()
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::R_SESSION_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::ALGORITHM, MANDATORY | READONLY).set_enum_definition(enums::repetition_algorithm_to_enum_definition()),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::QUESTION_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::REVIEW_DATE, DATETIME | READONLY),
            coldef(COLS::GRADE, INTEGER | READONLY),
            coldef(COLS::RESPONSE_DATA, READONLY),
            coldef(COLS::NOTES, READONLY),
            coldef(COLS::STARTED_AT, DATETIME | READONLY),
            coldef(COLS::ENDED_AT, DATETIME | READONLY),
            coldef(COLS::LATENCY_MS, INTEGER | READONLY),
            coldef(COLS::CHANGED_ANSWER, BOOL | READONLY).set_default_value(false),
            coldef(COLS::DETAILS_JSON, MANDATORY | READONLY)
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int r_session_id{};
        enums::RepetitionAlgorithm algorithm{};
        int note_id{};
        int question_id{};
        unixtime review_date;
        int grade{};
        string response_data;
        string notes;
        unixtime started_at;
        unixtime ended_at;
        int latency_ms{};
        bool changed_answer{false};
        string details_json{"{}"};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                r_session_id == other.r_session_id &&
                algorithm == other.algorithm &&
                note_id == other.note_id &&
                question_id == other.question_id &&
                review_date == other.review_date &&
                grade == other.grade &&
                response_data == other.response_data &&
                notes == other.notes &&
                started_at == other.started_at &&
                ended_at == other.ended_at &&
                latency_ms == other.latency_ms &&
                changed_answer == other.changed_answer &&
                details_json == other.details_json;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // RREVIEW_H
