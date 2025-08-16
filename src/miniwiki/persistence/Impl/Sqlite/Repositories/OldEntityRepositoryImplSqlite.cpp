///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Note management tool.
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

#include "miniwiki/persistence/Api/OldEntityRepository.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/OldEntityRepositoryImplSqlite.h"
#include "miniwiki/models/OldEntity.h"

#include <iostream>
#include <string>
#include <vector>

#include "miniwiki/Global.h"
#include "SQLiteCpp/Database.h"
#include "miniwiki/persistence/Impl/Sqlite/SqliteFileName.h"
#include "miniwiki/persistence/Impl/Sqlite/Tables/OldEntityTable.h"

namespace MiniWiki::Impl::Sqlite::Repositories
{
    using std::vector;
    using Persistence::Impl::Sqlite::Tables::OldEntityTable;
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    OldEntityRepositoryImplSqlite::OldEntityRepositoryImplSqlite() = default;

    OldEntityRepositoryImplSqlite::~OldEntityRepositoryImplSqlite() = default;

    void OldEntityRepositoryImplSqlite::create(const Entity::OldEntity& content)
    {
        std::string sql = "INSERT INTO " +
            std::string(OldEntityTable::TABLE_NAME) +
            "(" +
            std::string(OldEntityTable::ENTITY_NAME) + "," +
            std::string(OldEntityTable::ENTITY_ID) + "," +
                std::string(OldEntityTable::VALUE) + "," +
            std::string(OldEntityTable::ADDED_AT) +
                ")";
        sql += " VALUES (?,?,?,?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 0;
            query.bind(++i, content.entity_name);
            query.bind(++i, content.entity_id);
            query.bind(++i, content.value);
            query.bind(++i, static_cast<long>(content.added_at));
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    Entity::OldEntity OldEntityRepositoryImplSqlite::read(std::string& entity_name, unixtime added_at)
    {
        std::string sql = "SELECT * FROM " + std::string(OldEntityTable::TABLE_NAME) +
            " WHERE " + OldEntityTable::ENTITY_NAME + "=? AND " + OldEntityTable::ADDED_AT + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        query.bind(1, entity_name);
        query.bind(2, static_cast<long>(added_at));

        if (query.executeStep())
        {
            Entity::OldEntity content;
            content.id = query.getColumn(0).getString();
            content.entity_name = query.getColumn(1).getString();
            content.entity_id = query.getColumn(2).getString();
            content.value = query.getColumn(3).getString();
            content.added_at = query.getColumn(4).getInt64();
            return content;
        }

        throw std::runtime_error("Old entity not found");
    }

    void OldEntityRepositoryImplSqlite::remove(std::string& entity_name, unixtime added_at)
    {
        std::string sql = "DELETE FROM " + std::string(OldEntityTable::TABLE_NAME) +
            " WHERE " + OldEntityTable::ENTITY_NAME + "=? AND " + OldEntityTable::ADDED_AT + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, entity_name);
            query.bind(2, static_cast<long>(added_at));
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::vector<unixtime> OldEntityRepositoryImplSqlite::list_timestamps(std::string& entity_name, std::string& entity_id)
  {

        std::string sql = "SELECT ADDED_AT FROM OLD_CONTENT WHERE ENTITY_NAME = ? AND ENTITY_ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 1;

            query.bind(i++, entity_name);
            query.bind(i++, entity_id);

            std::vector<unixtime> timestamps;

            while (query.executeStep())
            {
                using std::string;

                timestamps.push_back(query.getColumn(0).getInt64());
            }

            return timestamps;
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }
}
