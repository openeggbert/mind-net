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
#include "hive/plugins/repetition/RepetitionPlugin.hpp"

// ***** MACROS : START *****
#define Model R18State
#define MODEL R18_STATE
#define COLS columns::R18StateColumns
#include "../columns/R18StateColumns.hpp"

// ***** MACROS : END *****

namespace hive::plugins::repetition::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def R18_STATE_DEFINITION =
        def(COLS::MODEL_NAME, REPETITION_PLUGIN_NAME)
        .set_group("Repetition", 200).set_title_column(COLS::STABILITY_TIMES_100).allow_reader_write()
        .set_rest_operations("rl")
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::ELIGIBLE, BOOL | INTERNAL).set_default_value(false),
            coldef(COLS::STABILITY_TIMES_100, INTEGER).set_default_value(100),
            coldef(COLS::LAST_INTERVAL_TIMES_100, INTEGER).set_default_value(0),
            coldef(COLS::REPETITIONS, INTEGER).set_default_value(0),
            coldef(COLS::LAPSES, INTEGER).set_default_value(0),
            coldef(COLS::NEXT_REVIEW, DATETIME),
            coldef(COLS::LAST_REVIEW, DATETIME),
            coldef(COLS::LAST_QUALITY, INTEGER).set_default_value(0),
            coldef(COLS::LAST_SEEN_SEMANTIC_VERSION, INTEGER).set_default_value(1),
            coldef(COLS::CONTENT_MODIFIED_SINCE_LAST_REVIEW, BOOL).set_default_value(false),
        });

    struct Model : hive::model::BaseModel
    {
        identification user_id{};
        identification note_id{};
        bool eligible{false};
        int stability_times_100{100};
        int last_interval_times_100{0};
        int repetitions{0};
        int lapses{0};
        unixtime next_review;
        unixtime last_review;
        int last_quality{0};
        i64 last_seen_semantic_version{};
        bool content_modified_since_last_review{false};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::note_id,
            &Model::eligible,
            &Model::stability_times_100,
            &Model::last_interval_times_100,
            &Model::repetitions,
            &Model::lapses,
            &Model::next_review,
            &Model::last_review,
            &Model::last_quality,
            &Model::last_seen_semantic_version,
            &Model::content_modified_since_last_review
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS