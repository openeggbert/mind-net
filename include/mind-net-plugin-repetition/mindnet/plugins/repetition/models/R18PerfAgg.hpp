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
#pragma once


#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model R18PerfAgg
#define MODEL R18_PERF_AGG
#define COLS columns::R18PerfAggColumns
#include "../columns/R18PerfAggColumns.hpp"
#include "mindnet/plugins/repetition/RepetitionPlugin.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R18_PERF_AGG_DEFINITION =
        def(COLS::MODEL_NAME, REPETITION_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Repetition", 200)
        .set_title_column(COLS::BIN_LOG_T_TIMES_100)
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::BIN_LOG_T_TIMES_100, INTEGER | MANDATORY),
            coldef(COLS::TOTAL, INTEGER | MANDATORY).set_default_value(0),
            coldef(COLS::CORRECT, INTEGER | MANDATORY).set_default_value(0),
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int bin_log_t_times_100{};
        int total{0};
        int correct{0};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                bin_log_t_times_100 == other.bin_log_t_times_100 &&
                total == other.total &&
                correct == other.correct;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
