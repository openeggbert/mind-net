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

#include "mindnet/persistence/impl/sqlite/repositories/ContentRepositoryImplSqlite.h"
#include "mindnet/models/Content.h"

#include <string>
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "SQLiteCpp/Database.h"
#include "mindnet/persistence/impl/sqlite/SqliteFileName.h"

namespace mindnet::impl::sqlite::repositories
{


    ContentRepositoryImplSqlite::ContentRepositoryImplSqlite() = default;

    ContentRepositoryImplSqlite::~ContentRepositoryImplSqlite() = default;

    int ContentRepositoryImplSqlite::create(const entity_fields& fields)
    {
        try
        {
            return persistence::impl::sqlite::create_model(fields, get_model_definition());
        }
        catch (std::exception& e)
        {
            return -1;
        }
    }

    entity_fields ContentRepositoryImplSqlite::read(const int id)
    {
        return persistence::impl::sqlite::read_model(get_model_definition(), id);

    }

    models::ModelDefinition& ContentRepositoryImplSqlite::get_model_definition()
    {
        return models::CONTENT_DEFINITION;
    }

    entity_fields ContentRepositoryImplSqlite::convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body)
    {
        throw std::runtime_error("Not implemented");
    }

    //
    // void ContentRepositoryImplSqlite::remove(int id)
    // {
    //     std::string sql = "DELETE FROM " + std::string(ContentTable::MODEL_NAME) +
    //         " WHERE " + ContentTable::ID + "=?";
    //
    //     SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    //     SQLite::Statement query(db, sql);
    //
    //     try
    //     {
    //         query.bind(1, id);
    //         query.exec();
    //     }
    //     catch (SQLite::Exception& e)
    //     {
    //         err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
    //         throw std::runtime_error(e.what());
    //     }
    // }
    //
    // void ContentRepositoryImplSqlite::update(Entity::Content& content)
    // {
    //     std::string sql = "UPDATE " + std::string(ContentTable::MODEL_NAME) +
    //         " SET " + ContentTable::VALUE + "=?" +
    //         " WHERE " + ContentTable::ID + "=?";
    //
    //     SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    //     SQLite::Statement query(db, sql);
    //
    //     try
    //     {
    //         int i = 0;
    //         query.bind(++i, content.value);
    //         query.bind(++i, content.id);
    //         query.exec();
    //     }
    //     catch (SQLite::Exception& e)
    //     {
    //         err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
    //         throw std::runtime_error(e.what());
    //     }
    // }
}
