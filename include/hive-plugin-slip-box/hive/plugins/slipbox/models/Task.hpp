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
#include "hive/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : START *****
#define Model Task
#define MODEL TASK
#define COLS columns::TaskColumns
#include "../columns/TaskColumns.hpp"
#include "hive/plugins/slipbox/enums/TaskStatus.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def TASK_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box #2", 1000).set_title_column(COLS::TITLE)
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

    struct Model : hive::model::BaseModel
    {
        identification note_id{};
        identification project_id{};
        string title;
        string description;
        int progress{};
        TaskStatus status{};
        bool important{false};
        bool as_soon_as_possible{false};
        unixtime start_date{};
        unixtime due_date{};
        unixtime completed_at{};
        identification created_by{};
        identification owner_id{};
        identification assigned_to{};
        string category;
        string context;
        string tags;
        bool is_public{false};
        identification parent_task_id{};
        identification blocked_by_task_id{};
        string related_tasks;

        static constexpr auto fields = std::make_tuple(
            &Model::note_id,
            &Model::project_id,
            &Model::title,
            &Model::description,
            &Model::progress,
            &Model::status,
            &Model::important,
            &Model::as_soon_as_possible,
            &Model::start_date,
            &Model::due_date,
            &Model::completed_at,
            &Model::created_by,
            &Model::owner_id,
            &Model::assigned_to,
            &Model::category,
            &Model::context,
            &Model::tags,
            &Model::is_public,
            &Model::parent_task_id,
            &Model::blocked_by_task_id,
            &Model::related_tasks
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS