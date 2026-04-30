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
#include "hive/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model JobRun
#define MODEL JOB_RUN
#define COLS columns::JobRunColumns
#include "../columns/JobRunColumns.hpp"
#include "hive/plugins/core/enums/Crudl.hpp"

// ***** MACROS : END *****

namespace hive::plugins::core::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();
    inline const def JOB_RUN_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Core", 500).set_cache_enabled(false)
        .set_columns({
            //
            coldef(COLS::JOB_ENTRY_ID, READONLY | MANDATORY | FOREIGN_KEY),
            coldef(COLS::STARTED_AT, DATETIME | READONLY | MANDATORY),
            coldef(COLS::FINISHED_AT, DATETIME | MANDATORY),
            coldef(COLS::SUCCESS, BOOL | MANDATORY).set_default_value(false),
            coldef(COLS::MESSAGE).set_default_value(""),
        });

    struct Model : hive::model::BaseModel
    {
        identification job_entry_id;
        unixtime started_at;
        unixtime finished_at;
        bool success{};
        string message{true};

        static constexpr auto fields = std::make_tuple(
            &Model::job_entry_id,
            &Model::started_at,
            &Model::finished_at,
            &Model::success,
            &Model::message
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS