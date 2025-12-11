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
#define Model DictionaryTermFulltext
#define MODEL DICTIONARY_TERM_FULLTEXT
#define COLS columns::DictionaryTermFulltextColumns
#include "../columns/DictionaryTermFulltextColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_FULLTEXT_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary", 100)
        .set_rest_operations("l").set_title_column(COLS::ID)
        .set_no_table(true)
        .set_cache_enabled(false)
        .set_columns({
            //
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::TITLE_PART, MANDATORY | READONLY),
            coldef(COLS::TITLE, MANDATORY | READONLY),
            coldef(COLS::DISAMBIGUATION),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification dictionary_term_id{};
        identification dictionary_map_id{};
        std::string title_part{};
        std::string title{};
        std::string disambiguation{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::dictionary_map_id,
            &Model::title_part,
            &Model::title,
            &Model::disambiguation
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                dictionary_map_id == other.dictionary_map_id &&
                dictionary_term_id == other.dictionary_term_id && title_part == other.title_part
                && title == other.title &&
                disambiguation == other.disambiguation &&
                created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
