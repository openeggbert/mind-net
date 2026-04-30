#pragma once

#include <string>
#include <utility>

#include "hive/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model DictionaryIndexType
#define MODEL DICTIONARY_INDEX_TYPE
#define COLS columns::DictionaryIndexTypeColumns
#include "../columns/DictionaryIndexTypeColumns.hpp"
// ***** MACROS : END *****

namespace hive::plugins::dictionary::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def DICTIONARY_INDEX_TYPE_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 200)
        .set_all_rest_operations()
        .set_columns({
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | READONLY | FOREIGN_KEY)
                .set_description("Parent dictionary map."),

            coldef(COLS::TITLE, MANDATORY)
                .set_description("Title of the dictionary index."),

            coldef(COLS::DESCRIPTION)
                .set_description("Optional description of the index."),

            coldef(COLS::POSITION, INTEGER)
                .set_description("Ordering position of the index within the map."),
        });

    struct DictionaryIndexType : hive::model::BaseModel
    {
        identification dictionary_map_id{};
        std::string title{};
        std::string description{};
        int position{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_map_id,
            &Model::title,
            &Model::description,
            &Model::position
        );

        create_model_h_methods(Model, MODEL)

    };
}

#undef Model
#undef MODEL
#undef COLS
