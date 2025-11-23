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


/**
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#pragma once


#include "mindnet/model/BaseColumns.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ColumnType.hpp"


namespace mindnet::plugins::slipbox::columns
{
    struct TaskColumns : model::BaseColumns
    {
        TaskColumns() = delete;

        TaskColumns(const TaskColumns&) = delete;
        TaskColumns& operator=(const TaskColumns&) = delete;

        static constexpr const char* MODEL_NAME = "task";

        // Foreign key columns
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* PROJECT_ID = "project_id";
        static constexpr const char* CREATED_BY = "created_by";
        static constexpr const char* OWNER_ID = "owner_id";
        static constexpr const char* ASSIGNED_TO = "assigned_to";
        static constexpr const char* PARENT_TASK_ID = "parent_task_id";
        static constexpr const char* BLOCKED_BY_TASK_ID = "blocked_by_task_id";

        // Content columns
        static constexpr const char* TITLE = "title";
        static constexpr const char* DESCRIPTION = "description";
        static constexpr const char* PROGRESS = "progress";

        // Task state columns
        static constexpr const char* STATUS = "status";
        static constexpr const char* IMPORTANT = "important";
        static constexpr const char* AS_SOON_AS_POSSIBLE = "as_soon_as_possible";
        static constexpr const char* START_DATE = "start_date";
        static constexpr const char* DUE_DATE = "due_date";
        static constexpr const char* COMPLETED_AT = "completed_at";

        // Metadata columns
        static constexpr const char* CATEGORY = "category";
        static constexpr const char* CONTEXT = "context";
        static constexpr const char* TAGS = "tags";
        static constexpr const char* IS_PUBLIC = "is_public";
        static constexpr const char* RELATED_TASKS = "related_tasks";
    };
}
