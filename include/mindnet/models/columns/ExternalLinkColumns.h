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
#ifndef EXTERNALLINKCOLUMNS_H
#define EXTERNALLINKCOLUMNS_H

#include "BaseColumns.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/ColumnType.h"


namespace mindnet::models::columns
{
    struct ExternalLinkColumns : BaseColumns
    {
        ExternalLinkColumns() = delete;

        ExternalLinkColumns(const ExternalLinkColumns&) = delete;
        ExternalLinkColumns& operator=(const ExternalLinkColumns&) = delete;

        static constexpr const char* MODEL_NAME = "external_link";

        static constexpr const char* FROM_NODE_ID = "from_node_id";
        static constexpr const char* TO_URL = "to_url";
    };
}
#endif // EXTERNALLINKCOLUMNS_H
