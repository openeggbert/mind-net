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
#include "mindnet/plugins/repetition/RepetitionPlugin.hpp"

// ***** MACROS : START *****
#define Model R18PredictionLog
#define MODEL R18_PREDICTION_LOG
#define COLS columns::R18PredictionLogColumns
#include "../columns/R18PredictionLogColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R18_PREDICTION_LOG_DEFINITION =
        def(COLS::MODEL_NAME, REPETITION_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Repetition", 210)
        .set_title_column(COLS::PREDICTED_R_TIMES_100)
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::PREDICTED_R_TIMES_100, INTEGER | MANDATORY),
            coldef(COLS::ACTUAL_GRADE, INTEGER | MANDATORY),
            coldef(COLS::WAS_CORRECT, BOOL | MANDATORY),
        });

    struct Model : mindnet::model::BaseModel
    {
        i64 user_id{};
        i64 note_id;
        i64 predicted_R_times_100{};
        i64 actual_grade{};
        bool was_correct{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                predicted_R_times_100 == other.predicted_R_times_100 &&
                actual_grade == other.actual_grade &&
                was_correct == other.was_correct;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
