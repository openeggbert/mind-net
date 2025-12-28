#pragma once

#include <utility>

#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model DictionaryIndex
#define MODEL DICTIONARY_INDEX
#define COLS columns::DictionaryIndexColumns
#include "../columns/DictionaryIndexColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_INDEX_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
            .set_group("Dictionary", 210)
            .set_all_rest_operations().set_readonly()
            .set_columns({
                coldef(COLS::DICTIONARY_INDEX_TYPE_ID, MANDATORY | FOREIGN_KEY)
                    .set_description("Dictionary index type acting as entry gate."),

                coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY)
                    .set_description("Dictionary term belonging to the index."),

                coldef(COLS::POSITION, INTEGER | MUTABLE).set_default_value(false)
                    .set_description("Optional ordering of the term inside the index."),

                coldef(COLS::IS_ENTRY_POINT, BOOL | MUTABLE).set_default_value(false)
                    .set_description("Marks the term as an entry point of the index."),
            });

    struct DictionaryIndex : mindnet::model::BaseModel
    {
        identification dictionary_index_type_id{};
        identification dictionary_term_id{};
        int position{};
        bool is_entry_point{false};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_index_type_id,
            &Model::dictionary_term_id,
            &Model::position,
            &Model::is_entry_point
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                   dictionary_index_type_id == other.dictionary_index_type_id &&
                   dictionary_term_id == other.dictionary_term_id &&
                   position == other.position &&
                   is_entry_point == other.is_entry_point &&
                   created_at == other.created_at &&
                   updated_at == other.updated_at;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
