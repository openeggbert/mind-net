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
#ifndef TAGTYPECOLUMNS_H
#define TAGTYPECOLUMNS_H

#include "../../../model/BaseColumns.h"
#include "mindnet/Helper.h"
#include "../../../model/ColumnType.h"

namespace mindnet::plugins::slipbox::columns
{
    struct TagTypeColumns : model::BaseColumns
    {
        TagTypeColumns() = delete;

        TagTypeColumns(const TagTypeColumns&) = delete;
        TagTypeColumns& operator=(const TagTypeColumns&) = delete;

        static constexpr const char* MODEL_NAME = "tag_type";

        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* TITLE = "title";
    };
}
#endif // TAGTYPECOLUMNS_H
