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
#ifndef NODEPROPERTYCOLUMNS_H
#define NODEPROPERTYCOLUMNS_H

#include "BaseColumns.h"


namespace mindnet::models::columns
{
    struct NodePropertyColumns : public BaseColumns
    {
        NodePropertyColumns() = delete;

        NodePropertyColumns(const NodePropertyColumns&) = delete;
        NodePropertyColumns& operator=(const NodePropertyColumns&) = delete;

        static constexpr const char* MODEL_NAME = "node_property";

        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* NODE_ID = "node_id";
        static constexpr const char* KEY = "key";
        static constexpr const char* VALUE = "value";
        static constexpr const char* VALUE_TYPE = "value_type";
        static constexpr const char* IS_INDEXED = "is_indexed";
    };
}
#endif // NODEPROPERTYCOLUMNS_H
