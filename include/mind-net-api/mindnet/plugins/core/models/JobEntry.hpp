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
#include "mindnet/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model JobEntry
#define MODEL JOB_ENTRY
#define COLS columns::JobEntryColumns
#include "../columns/JobEntryColumns.hpp"
#include "mindnet/plugins/core/enums/Crudl.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def JOB_ENTRY_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rudl")
        .set_group("Core", 500).set_cache_enabled(false)
        .set_columns({
            //
            coldef(COLS::NAME, READONLY | MANDATORY | UNIQUE),
            coldef(COLS::DESCRIPTION, READONLY |MANDATORY),
            coldef(COLS::PLUGIN_NAME, READONLY |MANDATORY),
            coldef(COLS::CRON_EXPRESSION, MANDATORY),
            coldef(COLS::RUN_ONCE_WHEN_MISSED, BOOL | READONLY | MANDATORY),
            coldef(COLS::ENABLED, BOOL | MANDATORY).set_default_value(true),
            coldef(COLS::CONFIGURATION).set_default_value(""),
            coldef(COLS::LAST_RUN, DATETIME | MANDATORY).set_default_value(0),
            coldef(COLS::NEXT_RUN, DATETIME | MANDATORY).set_default_value(0),
        });

    struct Model : mindnet::model::BaseModel
    {
        std::string name;
        std::string description;
        std::string plugin_name;
        std::string cron_expression;
        bool run_once_when_missed{};
        bool enabled{true};
        string configuration;
        unixtime last_run{0};
        unixtime next_run{0};

        static constexpr auto fields = std::make_tuple(
            &Model::name,
            &Model::description,
            &Model::plugin_name,
            &Model::cron_expression,
            &Model::run_once_when_missed,
            &Model::enabled,
            &Model::configuration,
            &Model::last_run,
            &Model::next_run
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                name == other.name &&
                description == other.description &&
                plugin_name == other.plugin_name &&
                cron_expression == other.cron_expression &&
                run_once_when_missed == other.run_once_when_missed &&
                enabled == other.enabled &&
                configuration == other.configuration &&
                last_run == other.last_run &&
                next_run == other.next_run;
        }

        [[nodiscard]]
        bool equals_or_differs_only_in_enabled_or_configuration(const Model& other) const
        {
            Model tmp = *this;
            tmp.enabled = other.enabled;
            tmp.configuration = other.configuration;

            return tmp == other;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS