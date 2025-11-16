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
