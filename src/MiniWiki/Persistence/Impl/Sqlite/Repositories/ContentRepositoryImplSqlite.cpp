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

#include "MiniWiki/Persistence/Api/ContentRepository.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Repositories/ContentRepositoryImplSqlite.h"
#include "MiniWiki/Entity/Content.h"

#include <iostream>
#include <string>
#include <vector>

#include "MiniWiki/Global.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Tables/ContentTable.h"
#include "SQLiteCpp/Database.h"
#include "MiniWiki/Persistence/Impl/Sqlite/SqliteFileName.h"

namespace MiniWiki::Impl::Sqlite::Repositories
{
    using std::vector;
    using Persistence::Impl::Sqlite::Tables::ContentTable;
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    ContentRepositoryImplSqlite::ContentRepositoryImplSqlite() = default;

    ContentRepositoryImplSqlite::~ContentRepositoryImplSqlite() = default;

    void ContentRepositoryImplSqlite::create(const Entity::Content& content)
    {
        std::string sql = "INSERT INTO " +
            std::string(ContentTable::TABLE_NAME) +
            "(" +
            std::string(ContentTable::ID) + "," +
            std::string(ContentTable::VALUE) + ")";
        sql += " VALUES (?,?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 0;
            query.bind(++i, content.id);
            query.bind(++i, content.value);
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    Entity::Content ContentRepositoryImplSqlite::read(std::string& id)
    {
        std::string sql = "SELECT * FROM " + std::string(ContentTable::TABLE_NAME) +
            " WHERE " + ContentTable::ID + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        query.bind(1, id);

        if (query.executeStep())
        {
            Entity::Content content;
            content.id = query.getColumn(0).getString();
            content.value = query.getColumn(1).getString();
            return content;
        }

        throw std::runtime_error("Content not found");
    }

    void ContentRepositoryImplSqlite::remove(std::string& id)
    {
        std::string sql = "DELETE FROM " + std::string(ContentTable::TABLE_NAME) +
            " WHERE " + ContentTable::ID + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, id);
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    void ContentRepositoryImplSqlite::update(Entity::Content& content)
    {
        std::string sql = "UPDATE " + std::string(ContentTable::TABLE_NAME) +
            " SET " + ContentTable::VALUE + "=?" +
            " WHERE " + ContentTable::ID + "=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 0;
            query.bind(++i, content.value);
            query.bind(++i, content.id);
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << sql << ": " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }
}
