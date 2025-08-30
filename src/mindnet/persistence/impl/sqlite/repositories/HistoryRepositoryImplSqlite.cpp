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

#include "mindnet/persistence/impl/sqlite/repositories/HistoryRepositoryImplSqlite.h"

#include <string>

#include "mindnet/enums/Crudl.h"
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "SQLiteCpp/Database.h"

namespace mindnet::impl::sqlite::repositories
{
    HistoryRepositoryImplSqlite::~HistoryRepositoryImplSqlite() = default;

    def_virtual_irepository_impl_cpp_methods(History, HISTORY)

    entity_fields HistoryRepositoryImplSqlite::convert_crow_json_rvalue_to_entity_fields(
        crow::json::rvalue& body, enums::Crudl crudl)
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(History)

        add_string(TABLE_NAME);
        add_int(RECORD_ID);
        add_int(OPERATION);
        add_string(DATA_JSON);
        add_optional_string(REASON, "");
        return fields;
    }
}
