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
#ifndef CONTENTCOLUMNS_H
#define CONTENTCOLUMNS_H
#include "../../../models/columns/BaseColumns.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/ColumnType.h"


namespace mindnet::plugins::zettelkasten::columns
{
    struct ContentFtsColumns : model::BaseColumns
    {
        ContentFtsColumns() = delete;

        ContentFtsColumns(const ContentFtsColumns&) = delete;
        ContentFtsColumns& operator=(const ContentFtsColumns&) = delete;

        static constexpr const char* MODEL_NAME = "content_fts";

        static constexpr const char* VALUE = "value";
        static constexpr const char* FORMAT = "format";
        static constexpr const char* VERSION = "version";
    };
}
#endif // CONTENTCOLUMNS_H
