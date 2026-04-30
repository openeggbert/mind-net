#pragma once

#include <string>
#include <utility>

#include "hive/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model DictionaryTermAlias
#define MODEL DICTIONARY_TERM_ALIAS
#define COLS columns::DictionaryTermAliasColumns
#include "../columns/DictionaryTermAliasColumns.hpp"
// ***** MACROS : END *****

namespace hive::plugins::dictionary::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_ALIAS_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 210)
        .set_rest_operations("crdl").set_readonly()
        .set_columns({
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY)
                .set_description("Primary dictionary term."),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY)
                .set_description("Map ID."),
            coldef(COLS::ALIAS, MANDATORY)
                .set_description("Alias (alternative name) for the dictionary term."),
        });

    struct DictionaryTermAlias : hive::model::BaseModel
    {
        identification dictionary_term_id{};
        identification dictionary_map_id{};
        std::string alias{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::dictionary_map_id,
            &Model::alias
        );

        create_model_h_methods(Model, MODEL)

    };
}

#undef Model
#undef MODEL
#undef COLS
