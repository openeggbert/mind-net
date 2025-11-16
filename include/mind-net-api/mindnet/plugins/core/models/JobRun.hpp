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

// ***** MACROS : START *****
#define Model JobRun
#define MODEL JOB_RUN
#define COLS columns::JobRunColumns
#include "../columns/JobRunColumns.hpp"
#include "mindnet/plugins/core/enums/Crudl.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def JOB_RUN_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("rl")
        .set_group("Core", 500)
        .set_columns({
            //
            coldef(COLS::JOB_ENTRY_ID, READONLY | MANDATORY | FOREIGN_KEY),
            coldef(COLS::STARTED_AT, DATETIME | READONLY | MANDATORY),
            coldef(COLS::FINISHED_AT, DATETIME | MANDATORY),
            coldef(COLS::SUCCESS, BOOL | MANDATORY).set_default_value(false),
            coldef(COLS::MESSAGE).set_default_value(""),
        });

    struct Model : mindnet::model::BaseModel
    {
        i64 jon_entry_id;
        unixtime started_at;
        unixtime finished_at;
        bool success{};
        string message{true};
        
        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                jon_entry_id == other.jon_entry_id &&
                started_at == other.started_at &&
                finished_at == other.finished_at &&
                success == other.success &&
                message == other.message;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
