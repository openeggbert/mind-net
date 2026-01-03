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
#include "mindnet/plugins/dictionary/enums/Importance.hpp"
#include "mindnet/plugins/dictionary/enums/TermStatus.hpp"

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
            coldef(COLS::DISAMBIGUATION),
            coldef(COLS::DEFINITION, TEXTAREA).set_description("Definition of the term."),
            coldef(COLS::STATUS).set_default_value(0).set_enum_definition(enums::term_status_to_enum_definition())
                                     .set_description("Status of the term."),
            coldef(COLS::IMPORTANCE).set_default_value(2).set_enum_definition(enums::importance_to_enum_definition())
                                     .set_description("Importance level of the term."),
            coldef(COLS::DIFFICULTY).set_default_value(2).set_enum_definition(enums::difficulty_to_enum_definition()).
                                     set_description("Difficulty level of the term."),
            coldef(COLS::IS_FOR_REPETITION, BOOL).set_default_value(true),
            coldef(COLS::IS_ROOT, BOOL).set_default_value(false),
        })
        .add_custom_list_action("dictionary_term_visit", "List term visits", {"dictionary_term_id", "{id}"})
        .add_custom_create_action("dictionary_term_visit", "Add term visit", {"dictionary_term_id", "{id}"})
        .add_custom_list_action("dictionary_tag", "List tags", {"dictionary_term_id", "{id}"})
        .add_custom_create_action("dictionary_tag", "Add tag", {"dictionary_term_id", "{id}"})
        .add_custom_list_action("dictionary_note", "List notes", {"dictionary_term_id", "{id}"})
        .add_custom_create_action("dictionary_note", "Add note", {"dictionary_term_id", "{id}"})
        .add_custom_read_action("dictionary_older_term", "Read older term", {"id", "{id}"})
        .add_custom_read_action("dictionary_newer_term", "Read newer term", {"id", "{id}"})
    ;

    struct DictionaryTerm : mindnet::model::BaseModel
    {
        identification dictionary_map_id{};
        string title;
        string disambiguation;
        string definition;
        enums::TermStatus status{enums::TermStatus::None};
        enums::Importance importance{enums::Importance::Medium};
        enums::Difficulty difficulty{enums::Difficulty::Medium};
        bool is_for_repetition{true};
        bool is_root{false};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_map_id,
            &Model::title,
            &Model::disambiguation,
            &Model::definition,
            &Model::status,
            &Model::importance,
            &Model::difficulty,
            &Model::is_for_repetition,
            &Model::is_root
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
