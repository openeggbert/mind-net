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
#define Model DictionaryPinnedTerm
#define MODEL DICTIONARY_PINNED_TERM
#define COLS columns::DictionaryPinnedTermColumns
#include "../columns/DictionaryPinnedTermColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_PINNED_TERM_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 200)
        .set_rest_operations("crdl").set_readonly()
        .set_columns({
            //
            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY).set_description(
                "Dictionary term that was PINNED."),
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY).set_description("User who pinned the term."),
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY)
        });

    struct DictionaryPinnedTerm : mindnet::model::BaseModel
    {
        identification dictionary_term_id{};
        identification user_id{};
        identification dictionary_map_id{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::user_id,
            &Model::dictionary_map_id
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS
