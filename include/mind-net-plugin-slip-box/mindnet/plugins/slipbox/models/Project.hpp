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
#define Model Project
#define MODEL PROJECT
#define COLS columns::ProjectColumns
#include "../columns/ProjectColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def PROJECT_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100).set_title_column(COLS::TITLE)
        .set_columns({
            coldef(COLS::NOTE_ID, INTEGER | FOREIGN_KEY).set_description("Note ID this project is linked to"),
            coldef(COLS::TITLE, MANDATORY).set_description("Project title"),
            coldef(COLS::DESCRIPTION).set_description("Project description"),
            coldef(COLS::PROGRESS, INTEGER).set_description("Project progress (0-100)"),
            coldef(COLS::IN_PROGRESS, BOOL).set_default_value(0).set_description("Whether project is in progress"),
            coldef(COLS::IMPORTANT, BOOL).set_default_value(0).set_description("Project importance flag"),
            coldef(COLS::DUE_DATE, DATETIME).set_description("Project due date"),
            coldef(COLS::CREATED_BY, READONLY).set_foreign_key("user").set_description("User who created the project"),
            coldef(COLS::OWNER_ID).set_foreign_key("user").set_description("Project owner"),
            coldef(COLS::ASSIGNED_TO).set_foreign_key("user").set_description("User assigned to the project"),
            coldef(COLS::CATEGORY).set_description("Project category"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(0).set_description("Whether this project is public")
        });

    struct Model : mindnet::model::BaseModel
    {
        int note_id{};
        string title;
        string description;
        int progress{};
        bool in_progress{false};
        bool important{false};
        unixtime due_date;
        int created_by{};
        int owner_id{};
        int assigned_to{};
        string category;
        bool is_public{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                note_id == other.note_id &&
                title == other.title &&
                description == other.description &&
                progress == other.progress &&
                in_progress == other.in_progress &&
                important == other.important &&
                due_date == other.due_date &&
                created_by == other.created_by &&
                owner_id == other.owner_id &&
                assigned_to == other.assigned_to &&
                category == other.category &&
                is_public == other.is_public;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
