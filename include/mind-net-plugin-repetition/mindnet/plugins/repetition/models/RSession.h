#ifndef RSESSION_H
#define RSESSION_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model RSession
#define MODEL R_SESSION
#define COLS columns::RSessionColumns
#include "../columns/RSessionColumns.h"
#include "mindnet/plugins/repetition/enums/RepetitionAlgorithm.h"
#include "mindnet/plugins/repetition/enums/RepetitionSchedule.h"
#include "mindnet/plugins/repetition/enums/RepetitionScope.h"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def R_SESSION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Repetition", 200)
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::MAP_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::CLONED_FROM_SESSION_ID, READONLY).set_foreign_key("r_session"),

            coldef(COLS::ALGORITHM, MANDATORY | READONLY).set_enum_definition(enums::repetition_algorithm_to_enum_definition()),
            coldef(COLS::SCHEDULE, MANDATORY | READONLY).set_enum_definition(enums::repetition_scope_to_enum_definition()),

            coldef(COLS::NOTES, BOOL | MANDATORY | READONLY).set_default_value(true),
            coldef(COLS::QUESTIONS, BOOL | MANDATORY | READONLY).set_default_value(true),
            coldef(COLS::SCOPE, MANDATORY | READONLY).set_enum_definition(enums::repetition_scope_to_enum_definition()),
            coldef(COLS::DESCRIPTION, READONLY),

            coldef(COLS::FILTER_UNDER_NOTE,READONLY).set_foreign_key("note"),
            coldef(COLS::FILTER_DATE_FROM, DATETIME | READONLY),
            coldef(COLS::FILTER_DATE_TO, DATETIME | READONLY),
            coldef(COLS::FILTER_TAG, READONLY).set_foreign_key("tag"),
            coldef(COLS::FILTER_COLLECTION, READONLY).set_foreign_key("collection"),

            coldef(COLS::SELECTED_ITEMS, MANDATORY).set_default_value("{}"),
            coldef(COLS::PINNED, BOOL).set_default_value(false)
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int map_id{};
        int cloned_from_session_id{};
        enums::RepetitionAlgorithm algorithm{};
        enums::RepetitionSchedule schedule{};
        bool notes{true};
        bool questions{true};
        enums::RepetitionScope scope{};
        std::string description;
        int filter_under_note{};
        unixtime filter_date_from{};
        unixtime filter_date_to{};
        int filter_tag{};
        int filter_collection{};
        //example: {"note_ids":[3,4,5,6,7], "question_ids":[3,4,6,7,8]}
        string selected_items{"{}"};
        bool pinned{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                map_id == other.map_id &&
                cloned_from_session_id == other.cloned_from_session_id &&
                algorithm == other.algorithm &&
                schedule == other.schedule &&
                notes == other.notes &&
                questions == other.questions &&
                scope == other.scope &&
                description == other.description &&
                filter_under_note == other.filter_under_note &&
                filter_date_from == other.filter_date_from &&
                filter_date_to == other.filter_date_to &&
                filter_tag == other.filter_tag &&
                filter_collection == other.filter_collection &&
                selected_items == other.selected_items&&
                pinned == other.pinned;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // RSESSION_H
