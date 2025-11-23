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
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : START *****
#define Model Task
#define MODEL TASK
#define COLS columns::TaskColumns
#include "../columns/TaskColumns.hpp"
#include "mindnet/plugins/slipbox/enums/TaskStatus.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def TASK_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box #2", 100).set_title_column(COLS::TITLE)
        .set_columns({
            coldef(COLS::NOTE_ID, INTEGER | FOREIGN_KEY).set_description("Note ID this task is linked to"),
            coldef(COLS::PROJECT_ID, INTEGER | FOREIGN_KEY).set_description(
                "Project ID this task belongs to"),
            coldef(COLS::TITLE, MANDATORY).set_description("Task title"),
            coldef(COLS::DESCRIPTION).set_description("Task description"),
            coldef(COLS::PROGRESS, INTEGER).set_description("Task progress (0-100)"),
            coldef(COLS::STATUS).set_enum_definition(task_status_to_enum_definition()).set_default_value(0).
                                 set_description("Task status"),
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
        TaskStatus status{};
        bool important{false};
        bool as_soon_as_possible{false};
        unixtime start_date;
        unixtime due_date;
        unixtime completed_at;
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
                as_soon_as_possible == other.as_soon_as_possible &&
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
