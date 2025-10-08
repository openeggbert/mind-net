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
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::MAP_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::CLONED_FROM_SESSION_ID, FOREIGN_KEY),
            coldef(COLS::ALGORITHM, MANDATORY).set_enum_definition(enums::repetition_algorithm_to_enum_definition()),
            coldef(COLS::NOTES, BOOL | MANDATORY).set_default_value(true),
            coldef(COLS::QUESTIONS, BOOL | MANDATORY).set_default_value(true),
            coldef(COLS::SCOPE, MANDATORY).set_enum_definition(enums::repetition_scope_to_enum_definition()),
            coldef(COLS::FILTER_UNDER_NOTE).set_foreign_key("note"),
            coldef(COLS::FILTER_DATE_FROM, DATETIME),
            coldef(COLS::FILTER_DATE_TO, DATETIME),
            coldef(COLS::FILTER_TAG).set_foreign_key("tag"),
            coldef(COLS::FILTER_COLLECTION, FOREIGN_KEY).set_foreign_key("collection"),
            coldef(COLS::SELECTED_ITEMS, MANDATORY).set_default_value("{}"),
            coldef(COLS::PINNED, BOOL).set_default_value(false)
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int map_id{};
        int cloned_from_session_id{};
        enums::RepetitionAlgorithm algorithm{};
        bool notes{true};
        bool questions{true};
        enums::RepetitionScope scope{};
        int filter_under_note{};
        unixtime filter_date_from{};
        unixtime filter_date_to{};
        int filter_tag{};
        int filter_collection{};
        string selected_items{"{}"};

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
                notes == other.notes &&
                questions == other.questions &&
                scope == other.scope &&
                filter_under_note == other.filter_under_note &&
                filter_date_from == other.filter_date_from &&
                filter_date_to == other.filter_date_to &&
                filter_tag == other.filter_tag &&
                filter_collection == other.filter_collection &&
                selected_items == other.selected_items;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // RSESSION_H
