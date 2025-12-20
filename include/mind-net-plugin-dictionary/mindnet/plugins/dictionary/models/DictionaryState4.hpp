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
#define Model DictionaryState4
#define MODEL DICTIONARY_STATE_4
#define COLS columns::DictionaryState4Columns
#include "../columns/DictionaryState4Columns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_STATE_4_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary", 100)
        .set_title_column(COLS::DICTIONARY_TERM_ID)
        .set_rest_operations("rl")
        .allow_reader_write()
    .set_columns({
        coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
        coldef(COLS::DICTIONARY_TERM_ID, FOREIGN_KEY | MANDATORY | READONLY),

        coldef(COLS::REPETITIONS, INTEGER).set_default_value(0),
        coldef(COLS::INTERVAL, INTEGER).set_default_value(1),
        coldef(COLS::EF_TIMES_100, INTEGER).set_default_value(250),
        coldef(COLS::CORRECTION_FACTOR_TIMES_100, INTEGER).set_default_value(100),

        coldef(COLS::NEXT_REVIEW, DATETIME),
        coldef(COLS::LAST_REVIEW, DATETIME),
        coldef(COLS::LAST_QUALITY, INTEGER).set_default_value(0),
      
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        identification dictionary_term_id{};
        int repetitions{};
        int interval{1};
        int ef_times_100{250};
        int correction_factor_times_100{100};
        unixtime next_review;
        unixtime last_review;
        int last_quality{};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::dictionary_term_id,
            &Model::repetitions,
            &Model::interval,
            &Model::ef_times_100,
            &Model::correction_factor_times_100,
            &Model::next_review,
            &Model::last_review,
            &Model::last_quality
        );
        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                dictionary_term_id == other.dictionary_term_id &&
                repetitions == other.repetitions &&
                interval == other.interval &&
                ef_times_100 == other.ef_times_100 &&
                correction_factor_times_100 == other.correction_factor_times_100 &&
                next_review == other.next_review &&
                last_review == other.last_review &&
                last_quality == other.last_quality;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS