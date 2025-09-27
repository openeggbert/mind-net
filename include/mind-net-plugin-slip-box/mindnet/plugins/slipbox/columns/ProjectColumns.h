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
#ifndef PROJECTCOLUMNS_H
#define PROJECTCOLUMNS_H

#include "mindnet/model/BaseColumns.h"
#include "mindnet/essential/Helper.h"
#include "mindnet/model/ColumnType.h"

namespace mindnet::plugins::slipbox::columns
{
    struct ProjectColumns : model::BaseColumns
    {
        ProjectColumns() = delete;

        ProjectColumns(const ProjectColumns&) = delete;
        ProjectColumns& operator=(const ProjectColumns&) = delete;

        static constexpr const char* MODEL_NAME = "project";

        // Foreign key columns
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* CREATED_BY = "created_by";
        static constexpr const char* OWNER_ID = "owner_id";
        static constexpr const char* ASSIGNED_TO = "assigned_to";

        // Content columns
        static constexpr const char* TITLE = "title";
        static constexpr const char* DESCRIPTION = "description";
        static constexpr const char* PROGRESS = "progress";

        // Project state columns
        static constexpr const char* IN_PROGRESS = "in_progress";
        static constexpr const char* IMPORTANT = "important";
        static constexpr const char* DUE_DATE = "due_date";

        // Metadata columns
        static constexpr const char* CATEGORY = "category";
        static constexpr const char* IS_PUBLIC = "is_public";
    };
}
#endif // PROJECTCOLUMNS_H
