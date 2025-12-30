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

#include "mindnet/model/BaseModel.hpp"
#include "mindnet/plugins/dictionary/DictionaryPlugin.hpp"
// ***** MACROS : START *****
#define Model DictionaryFlag
#define MODEL DICTIONARY_FLAG
#define COLS columns::DictionaryFlagColumns
#include "../columns/DictionaryFlagColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_FLAG_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary", 100)
        .set_rest_operations("crdl").set_title_column(COLS::TITLE).set_readonly()
        .set_columns({
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::TITLE, MANDATORY),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(false),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification dictionary_term_id{0};
        identification user_id{0};
        identification dictionary_map_id{0};
        std::string title;
        bool is_public{false};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::user_id,
            &Model::dictionary_map_id,
            &Model::title,
            &Model::is_public
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
