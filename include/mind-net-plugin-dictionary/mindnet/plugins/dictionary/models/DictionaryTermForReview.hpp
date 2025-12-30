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
#define Model DictionaryTermForReview
#define MODEL DICTIONARY_TERM_FOR_REVIEW
#define COLS columns::DictionaryTermForReviewColumns
#include "../columns/DictionaryTermForReviewColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_FOR_REVIEW_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary #2", 200)
        .set_rest_operations("l").set_title_column(COLS::TITLE)
        .set_no_table(true)
        .set_cache_enabled(false).set_readonly()
        .set_columns({
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY),
            coldef(COLS::DICTIONARY_SEARCH_ID, FOREIGN_KEY),

            coldef(COLS::TITLE, MANDATORY),
            coldef(COLS::DISAMBIGUATION),
            coldef(COLS::DEFINITION),

            coldef(COLS::ALGORITHM, INTEGER),
            coldef(COLS::IS_DUE, BOOL).set_default_value(true),
            coldef(COLS::IS_NOT_DUE, BOOL).set_default_value(false),
            coldef(COLS::IS_NEVER, BOOL).set_default_value(true),
            coldef(COLS::HAS_DEFINITION, BOOL).set_default_value(true),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification dictionary_term_id{};
        identification dictionary_map_id{};
        identification user_id{};
        identification dictionary_search_id{};

        string title;
        string disambiguation;
        string definition;

        int algorithm{};
        bool is_due{false};
        bool is_never{false};
        bool is_not_due{false};
        bool has_definition{false};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::dictionary_map_id,
            &Model::user_id,
            &Model::dictionary_search_id,
            &Model::title,
            &Model::disambiguation,
            &Model::definition,
            &Model::algorithm,
            &Model::is_due,
            &Model::is_never,
            &Model::is_not_due,
            &Model::has_definition
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
