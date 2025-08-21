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

#include "mindnet/persistence/impl/sqlite/repositories/MapRepositoryImplSqlite.h"
#include "mindnet/models/Map.h"

#include <string>
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "SQLiteCpp/Database.h"
#include "mindnet/persistence/impl/sqlite/SqliteFileName.h"

namespace mindnet::impl::sqlite::repositories
{

    MapRepositoryImplSqlite::~MapRepositoryImplSqlite() = default;
    int MapRepositoryImplSqlite::create(const entity_fields& fields)
    {
        try
        {
            return persistence::impl::sqlite::create_model(fields, get_model_definition());
        }
        catch (std::exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            return -1;
        }
    }

    entity_fields MapRepositoryImplSqlite::read(int id)
    {
        return persistence::impl::sqlite::read_model(get_model_definition(), id);
    }

    models::ModelDefinition& MapRepositoryImplSqlite::get_model_definition()
    {
        return models::MAP_DEFINITION;
    }

    entity_fields MapRepositoryImplSqlite::convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body)
    {
        typedef models::columns::MapColumns cols;
        entity_fields fields;
        fields.push_back(0);
        fields.push_back(body[cols::NAME].s());
        fields.push_back(body[cols::DESCRIPTION].s());
        fields.push_back(static_cast<int64_t>(Utils::currentUnixTimestamp()));
        return fields;
    }

    bool MapRepositoryImplSqlite::update(int id, entity_fields& fields)
    {
        return persistence::impl::sqlite::update_model(id, get_model_definition(), fields);
    }

    bool MapRepositoryImplSqlite::remove(int id)
    {
        return persistence::impl::sqlite::delete_model(get_model_definition(), id);
    }

}
