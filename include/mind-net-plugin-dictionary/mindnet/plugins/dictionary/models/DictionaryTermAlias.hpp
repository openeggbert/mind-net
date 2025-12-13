#pragma once

#include <string>
#include <utility>

#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model DictionaryTermAlias
#define MODEL DICTIONARY_TERM_ALIAS
#define COLS columns::DictionaryTermAliasColumns
#include "../columns/DictionaryTermAliasColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_ALIAS_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 210)
        .set_rest_operations("crdl")
        .set_columns({
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | READONLY | FOREIGN_KEY)
                .set_description("Primary dictionary term."),
            coldef(COLS::ALIAS, MANDATORY | READONLY)
                .set_description("Alias (alternative name) for the dictionary term."),
        });

    struct DictionaryTermAlias : mindnet::model::BaseModel
    {
        identification dictionary_term_id{};
        std::string alias{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::alias
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                   dictionary_term_id == other.dictionary_term_id &&
                   alias == other.alias &&
                   created_at == other.created_at &&
                   updated_at == other.updated_at;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
