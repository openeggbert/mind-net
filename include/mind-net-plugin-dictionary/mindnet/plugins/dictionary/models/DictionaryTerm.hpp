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

// ***** MACROS : START *****
#define Model DictionaryTerm
#define MODEL DICTIONARY_TERM
#define COLS columns::DictionaryTermColumns
#include "../columns/DictionaryTermColumns.hpp"
// ***** MACROS : END *****

#include "../enums/Difficulty.hpp"

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 200)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description(
                "Dictionary map this term belongs to."),
            coldef(COLS::TITLE, MANDATORY).set_description("Title of the dictionary term."),
            coldef(COLS::DEFINITION, TEXTAREA).set_description("Definition of the term."),
            coldef(COLS::DIFFICULTY).set_default_value(2).set_enum_definition(enums::difficulty_to_enum_definition()).
                                     set_description("Difficulty level of the term."),
        });

    struct DictionaryTerm : mindnet::model::BaseModel
    {
        identification dictionary_map_id{};
        string title;
        string definition;
        enums::Difficulty difficulty{enums::Difficulty::Medium};

        static constexpr auto fields = std::make_tuple(
            &Model::id,
            &Model::created_at,
            &Model::updated_at,

            &Model::dictionary_map_id,
            &Model::title,
            &Model::definition,
            &Model::difficulty
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                dictionary_map_id == other.dictionary_map_id &&
                title == other.title &&
                definition == other.definition &&
                difficulty == other.difficulty &&
                created_at == other.created_at &&
                updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
