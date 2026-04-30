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

#include "hive/model/BaseModel.hpp"
#include "hive/plugins/dictionary/DictionaryPlugin.hpp"
// ***** MACROS : START *****
#define Model DictionaryNote
#define MODEL DICTIONARY_NOTE
#define COLS columns::DictionaryNoteColumns
#include "../columns/DictionaryNoteColumns.hpp"

// ***** MACROS : END *****

namespace hive::plugins::dictionary::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def DICTIONARY_NOTE_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary", 100)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_columns({
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::TITLE, MANDATORY),
            coldef(COLS::CONTENT, TEXTAREA),
            coldef(COLS::POSITION, INTEGER).set_default_value(0),
        });

    struct Model : hive::model::BaseModel
    {
        identification dictionary_term_id {};
        string title;
        string content;
        int position{0};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::title,
            &Model::content,
            &Model::position
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
