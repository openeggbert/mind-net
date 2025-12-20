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
        identification user_id{};
        int bin_log_t_times_100{};
        int total{0};
        int correct{0};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::bin_log_t_times_100,
            &Model::total,
            &Model::correct
        );

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