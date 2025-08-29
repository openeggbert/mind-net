///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef MAPCOLUMNS_H
#define MAPCOLUMNS_H

#include "BaseColumns.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/ColumnType.h"


namespace mindnet::models::columns
{
    struct MapColumns : BaseColumns
    {
        MapColumns() = delete;

        MapColumns(const MapColumns&) = delete;
        MapColumns& operator=(const MapColumns&) = delete;

        static constexpr const char* MODEL_NAME = "map";

        static constexpr const char* NAME = "name";
        static constexpr const char* DESCRIPTION = "description";
        static constexpr const char* CATEGORY = "category";
        static constexpr const char* OWNER_ID = "owner_id";
        static constexpr const char* TEAM_ID = "team_id";
        static constexpr const char* OWNER_RIGHTS = "owner_rights";
        static constexpr const char* TEAM_RIGHTS = "team_rights";
        static constexpr const char* OTHER_RIGHTS = "other_rights";
    };
}
#endif // MAPCOLUMNS_H
