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
#define Model DictionaryUrlTypeFulltext
#define MODEL DICTIONARY_URL_TYPE_FULLTEXT
#define COLS columns::DictionaryUrlTypeFulltextColumns
#include "../columns/DictionaryUrlTypeFulltextColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_URL_TYPE_FULLTEXT_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary #2", 100)
        .set_rest_operations("l").set_title_column(COLS::ID)
        .set_no_table(true)
        .set_cache_enabled(false).set_readonly()
        .set_columns({
            //
            coldef(COLS::DICTIONARY_URL_TYPE_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::URL_PART, MANDATORY),
            coldef(COLS::URL, MANDATORY),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification dictionary_url_type_id{};
        identification dictionary_map_id{};
        std::string url_part{};
        std::string url{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_url_type_id,
            &Model::dictionary_map_id,
            &Model::url_part,
            &Model::url
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
