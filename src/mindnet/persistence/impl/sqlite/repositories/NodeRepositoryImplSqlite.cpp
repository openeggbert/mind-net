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

#include "mindnet/persistence/impl/sqlite/repositories/NodeRepositoryImplSqlite.h"

#include <string>

#include "mindnet/enums/Crudl.h"
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "SQLiteCpp/Database.h"

namespace mindnet::impl::sqlite::repositories
{
    NodeRepositoryImplSqlite::~NodeRepositoryImplSqlite() = default;

    def_virtual_irepository_impl_cpp_methods(Node, NODE)

    entity_fields NodeRepositoryImplSqlite::convert_crow_json_rvalue_to_entity_fields(
        crow::json::rvalue& body, enums::Crudl crudl)
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Node)

        add_string(UUID);
        add_int(MAP_ID);
        add_int(SIBLING_POSITION);
        add_string(TITLE);
        add_int(CONTENT_ID);
        add_int(PARENT_NODE_ID);
        add_int(TYPE);
        add_int(VISIBILITY);
        add_string(LAST_SHOWN_AT);
        add_string(EXPIRES_AT);
        add_int(IS_FAVORITE);
        add_int(IS_REDIRECT);
        add_int(REDIRECT_NODE_ID);
        add_string(REDIRECT_REASON);
        add_int(IMPORTANCE);
        add_int(DIFFICULTY);
        return fields;
    }

}
