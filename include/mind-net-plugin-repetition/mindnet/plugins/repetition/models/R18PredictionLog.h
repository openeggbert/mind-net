///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/> or write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef R18_PREDICTION_LOG_H
#define R18_PREDICTION_LOG_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model R18PredictionLog
#define MODEL R18_PREDICTION_LOG
#define COLS columns::R18PredictionLogColumns
#include "../columns/R18PredictionLogColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def R18_PREDICTION_LOG_DEFINITION =
        def(COLS::MODEL_NAME)
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
#endif // R18_PREDICTION_LOG_H
