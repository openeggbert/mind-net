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
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef MIGRATIONCOLUMNS_H
#define MIGRATIONCOLUMNS_H

namespace mindnet::orm
{
    struct SchemaHistoryColumns
    {
        SchemaHistoryColumns() = delete;

        SchemaHistoryColumns(const SchemaHistoryColumns&) = delete;

        SchemaHistoryColumns& operator=(const SchemaHistoryColumns&) = delete;

        static constexpr const char* MODEL_NAME = "schema_history";

        static constexpr const char* PLUGIN_NAME = "plugin_name";
        static constexpr const char* VERSION = "version";
        static constexpr const char* DESCRIPTION = "description";

        static constexpr const char* SCRIPT = "script";
        static constexpr const char* HASH_SHA256 = "checksum";
        static constexpr const char* CHAIN_HASH = "chain_hash";
        static constexpr const char* INSTALLED_ON = "installed_on";

        static constexpr const char* EXECUTION_TIME = "execution_time";
        static constexpr const char* SUCCESS = "success";
    };
}
#endif // MIGRATIONCOLUMNS_H
