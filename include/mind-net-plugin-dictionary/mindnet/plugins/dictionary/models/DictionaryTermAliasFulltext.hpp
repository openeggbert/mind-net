#pragma once

#include <string>
#include <utility>

#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model DictionaryTermAliasFulltext
#define MODEL DICTIONARY_TERM_ALIAS_FULLTEXT
#define COLS columns::DictionaryTermAliasFulltextColumns
#include "../columns/DictionaryTermAliasFulltextColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_ALIAS_FULLTEXT_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary #2", 210)
        .set_rest_operations("l")
        .set_no_table(true)
        .set_cache_enabled(false).set_readonly()
        .set_columns({
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY)
                .set_description("Map ID."),
            coldef(COLS::TITLE, MANDATORY),
            coldef(COLS::TITLE_PART, MANDATORY)
        });

    struct DictionaryTermAliasFulltext : mindnet::model::BaseModel
    {
        identification dictionary_map_id{};
        std::string title{};
        std::string title_part{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_map_id,
            &Model::title,
            &Model::title_part
        );

        create_model_h_methods(Model, MODEL)

    };
}

#undef Model
#undef MODEL
#undef COLS
