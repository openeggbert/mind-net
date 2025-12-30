/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <string>
#include <utility>

#include "mindnet/model/BaseModel.hpp"
#include "mindnet/plugins/dictionary/DictionaryPlugin.hpp"
#include "../enums/Difficulty.hpp"
#include "mindnet/plugins/dictionary/enums/Importance.hpp"
#include "mindnet/plugins/dictionary/enums/TermStatus.hpp"

// ***** MACROS : START *****
#define Model DictionaryTermSearch
#define MODEL DICTIONARY_TERM_SEARCH
#define COLS columns::DictionaryTermSearchColumns
#include "../columns/DictionaryTermSearchColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_SEARCH_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary #2", 100)
        .set_rest_operations("l").set_title_column(COLS::ID)
        .set_no_table(true)
        .set_cache_enabled(false).set_readonly()
        .set_columns({
            //
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::TITLE, MANDATORY),
            coldef(COLS::DISAMBIGUATION, MUTABLE),
            coldef(COLS::TERM_CREATED_AT, MANDATORY | DATETIME),
            coldef(COLS::TERM_UPDATED_AT, MANDATORY | DATETIME),
            coldef(COLS::STATUS, MUTABLE).set_default_value(0).set_enum_definition(enums::term_status_to_enum_definition())
                                .set_description("Status of the term."),
            coldef(COLS::IMPORTANCE, MUTABLE).set_default_value(2).set_enum_definition(enums::importance_to_enum_definition())
                                    .set_description("Importance level of the term."),
            coldef(COLS::DIFFICULTY, MUTABLE).set_default_value(2).set_enum_definition(enums::difficulty_to_enum_definition()).
                                     set_description("Difficulty level of the term."),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification dictionary_term_id{};
        identification dictionary_map_id{};
        std::string title{};
        std::string disambiguation{};
        unixtime term_created_at{};
        unixtime term_updated_at{};
        enums::TermStatus status{enums::TermStatus::None};
        enums::Importance importance{enums::Importance::Medium};
        enums::Difficulty difficulty{enums::Difficulty::Medium};


        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::dictionary_map_id,
            &Model::title,
            &Model::disambiguation,
            &Model::term_created_at,
            &Model::term_updated_at,
            &Model::status,
            &Model::importance,
            &Model::difficulty
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
