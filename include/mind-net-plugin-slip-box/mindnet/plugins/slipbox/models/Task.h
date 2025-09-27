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
#ifndef TASK_H
#define TASK_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model Task
#define MODEL TASK
#define COLS columns::TaskColumns
#include "../columns/TaskColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def TASK_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100).set_title_column(COLS::TITLE)
        .set_columns({
            coldef(COLS::NOTE_ID, INTEGER).set_foreign_key("note").set_description("Note ID this task is linked to"),
            coldef(COLS::PROJECT_ID, INTEGER).set_foreign_key("project").set_description(
                "Project ID this task belongs to"),
            coldef(COLS::TITLE, MANDATORY).set_description("Task title"),
            coldef(COLS::DESCRIPTION).set_description("Task description"),
            coldef(COLS::PROGRESS, INTEGER).set_description("Task progress (0-100)"),
            coldef(COLS::STATUS, INTEGER).set_default_value(0).set_description("Task status"),
            coldef(COLS::IMPORTANT, BOOL).set_default_value(0).set_description("Task importance flag"),
            coldef(COLS::AS_SOON_AS_POSSIBLE, BOOL).set_default_value(0).set_description("As soon as possible"),
            coldef(COLS::START_DATE, DATETIME).set_description("Task start date"),
            coldef(COLS::DUE_DATE, DATETIME).set_description("Task due date"),
            coldef(COLS::COMPLETED_AT, DATETIME).set_description("Task completion timestamp"),
            coldef(COLS::CREATED_BY, READONLY).set_foreign_key("user").set_description("User who created the task"),
            coldef(COLS::OWNER_ID).set_foreign_key("user").set_description("Task owner"),
            coldef(COLS::ASSIGNED_TO).set_foreign_key("user").set_description("User assigned to the task"),
            coldef(COLS::CATEGORY).set_description("Task category"),
            coldef(COLS::CONTEXT).set_description("Task context"),
            coldef(COLS::TAGS).set_description("Task tags"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(0).set_description("Whether this task is public"),
            coldef(COLS::PARENT_TASK_ID).set_foreign_key("task").set_description("Parent task ID"),
            coldef(COLS::BLOCKED_BY_TASK_ID).set_foreign_key("task").set_description("Blocking task ID"),
            coldef(COLS::RELATED_TASKS).set_description("Related task IDs")
        });

    struct Model : mindnet::model::BaseModel
    {
        int note_id{};
        int project_id{};
        string title;
        string description;
        int progress{};
        int status{};
        bool important{false};
        bool as_soon_possible{false};
        string start_date;
        string due_date;
        string completed_at;
        int created_by{};
        int owner_id{};
        int assigned_to{};
        string category;
        string context;
        string tags;
        bool is_public{false};
        int parent_task_id{};
        int blocked_by_task_id{};
        string related_tasks;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                note_id == other.note_id &&
                project_id == other.project_id &&
                title == other.title &&
                description == other.description &&
                progress == other.progress &&
                status == other.status &&
                important == other.important &&
                as_soon_possible == other.as_soon_possible &&
                start_date == other.start_date &&
                due_date == other.due_date &&
                completed_at == other.completed_at &&
                created_by == other.created_by &&
                owner_id == other.owner_id &&
                assigned_to == other.assigned_to &&
                category == other.category &&
                context == other.context &&
                tags == other.tags &&
                is_public == other.is_public &&
                parent_task_id == other.parent_task_id &&
                blocked_by_task_id == other.blocked_by_task_id &&
                related_tasks == other.related_tasks;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // TASK_H
