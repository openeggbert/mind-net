///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
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
        .set_group("Core", 500).set_read_cache_enabled(false)
        .set_columns({
            //
            coldef(COLS::NAME, READONLY | MANDATORY | UNIQUE),
            coldef(COLS::DESCRIPTION, MANDATORY),
            coldef(COLS::PLUGIN_NAME, MANDATORY),
            coldef(COLS::CRON_EXPRESSION, MANDATORY),
            coldef(COLS::RUN_ONCE_WHEN_MISSED, BOOL | MANDATORY),
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
