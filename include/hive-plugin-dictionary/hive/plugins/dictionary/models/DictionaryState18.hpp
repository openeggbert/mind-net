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
#define Model DictionaryState18
#define MODEL DICTIONARY_STATE_18
#define COLS columns::DictionaryState18Columns
#include "../columns/DictionaryState18Columns.hpp"

// ***** MACROS : END *****

namespace hive::plugins::dictionary::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def DICTIONARY_STATE_18_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary", 100)
        .set_title_column(COLS::DICTIONARY_TERM_ID)
        .set_rest_operations("rl")
        .allow_reader_write()
    .set_columns({
        coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
        coldef(COLS::DICTIONARY_TERM_ID, FOREIGN_KEY | MANDATORY | READONLY),

        coldef(COLS::STABILITY_TIMES_100, INTEGER).set_default_value(100),
        coldef(COLS::LAST_INTERVAL_TIMES_100, INTEGER).set_default_value(0),
        coldef(COLS::REPETITIONS, INTEGER).set_default_value(0),
        coldef(COLS::LAPSES, INTEGER).set_default_value(0),
        coldef(COLS::NEXT_REVIEW, DATETIME),
        coldef(COLS::LAST_REVIEW, DATETIME),
        coldef(COLS::LAST_QUALITY, INTEGER).set_default_value(0),
        });

    struct Model : hive::model::BaseModel
    {
        identification user_id{};
        identification dictionary_term_id{};
        int stability_times_100{100};
        int last_interval_times_100{0};
        int repetitions{0};
        int lapses{0};
        unixtime next_review{0};
        unixtime last_review{0};
        int last_quality{0};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::dictionary_term_id,
            &Model::stability_times_100,
            &Model::last_interval_times_100,
            &Model::repetitions,
            &Model::lapses,
            &Model::next_review,
            &Model::last_review,
            &Model::last_quality
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS