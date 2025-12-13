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

// ***** MACROS : START *****
#define Model DictionarySource
#define MODEL DICTIONARY_SOURCE
#define COLS columns::DictionarySourceColumns
#include "../columns/DictionarySourceColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def DICTIONARY_SOURCE_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Dictionary", 100)
        .set_title_column(COLS::DICTIONARY_SOURCE_TYPE_ID)
        .set_columns({
            //
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_SOURCE_TYPE_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::PAGE, INTEGER),
            coldef(COLS::NOTE),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification dictionary_term_id;
        identification dictionary_source_type_id;
        string page;
        string note;

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::dictionary_source_type_id,
            &Model::page,
            &Model::note
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id
                && dictionary_term_id == other.dictionary_term_id
                && dictionary_source_type_id == other.dictionary_source_type_id
                && page == other.page
                && note == other.note
                && created_at == other.created_at
                && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS