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

#include "MiniWiki/Persistence/Api/OldContentRepository.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Repositories/OldContentRepositoryImplSqlite.h"
#include "MiniWiki/Entity/OldContent.h"

#include <iostream>
#include <string>
#include <vector>

#include "MiniWiki/Global.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Tables/ContentTable.h"
#include "SQLiteCpp/Database.h"
#include "MiniWiki/Persistence/Impl/Sqlite/SqliteFileName.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Tables/OldContentTable.h"

namespace MiniWiki::Impl::Sqlite::Repositories
{
    using std::vector;
    using Persistence::Impl::Sqlite::Tables::OldContentTable;
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    OldContentRepositoryImplSqlite::OldContentRepositoryImplSqlite() = default;

    OldContentRepositoryImplSqlite::~OldContentRepositoryImplSqlite() = default;

    void OldContentRepositoryImplSqlite::create(const Entity::OldContent& content)
    {
        std::string sql = "INSERT INTO " +
            std::string(OldContentTable::TABLE_NAME) +
            "(" +
            std::string(OldContentTable::ID) + "," +
                std::string(OldContentTable::VALUE) + "," +
            std::string(OldContentTable::ADDED_AT) +
                ")";
        sql += " VALUES (?,?,?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 0;
            query.bind(++i, content.id);
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

    Entity::OldContent OldContentRepositoryImplSqlite::read(std::string& id, unixtime added_at)
    {
        std::string sql = "SELECT * FROM " + std::string(OldContentTable::TABLE_NAME) +
            " WHERE " + OldContentTable::ID + "=? AND " + OldContentTable::ADDED_AT + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        query.bind(1, id);
        query.bind(2, static_cast<long>(added_at));

        if (query.executeStep())
        {
            Entity::OldContent content;
            content.id = query.getColumn(0).getString();
            content.value = query.getColumn(1).getString();
            content.added_at = query.getColumn(2).getInt64();
            return content;
        }

        throw std::runtime_error("Content not found");
    }

    void OldContentRepositoryImplSqlite::remove(std::string& id, unixtime added_at)
    {
        std::string sql = "DELETE FROM " + std::string(OldContentTable::TABLE_NAME) +
            " WHERE " + OldContentTable::ID + "=? AND " + OldContentTable::ADDED_AT + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, id);
            query.bind(2, static_cast<long>(added_at));
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::vector<unixtime> OldContentRepositoryImplSqlite::list_timestamps(std::string& id)
  {

        std::string sql = "SELECT ADDED_AT FROM OLD_CONTENT WHERE ID = ? ";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 1;

            query.bind(i++, id);

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
