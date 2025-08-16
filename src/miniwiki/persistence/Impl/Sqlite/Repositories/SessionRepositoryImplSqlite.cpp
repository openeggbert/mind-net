///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki: Note management tool.
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

#include "miniwiki/persistence/Api/SessionRepository.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/SessionRepositoryImplSqlite.h"

#include <iostream>
#include <string>
#include <vector>

#include "miniwiki/Global.h"
#include "SQLiteCpp/Database.h"
#include "miniwiki/persistence/Impl/Sqlite/SqliteFileName.h"

namespace MiniWiki::Impl::Sqlite::Repositories
{
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    SessionRepositoryImplSqlite::SessionRepositoryImplSqlite() = default;

    SessionRepositoryImplSqlite::~SessionRepositoryImplSqlite() = default;

    void SessionRepositoryImplSqlite::create(const Entity::Session& session)
    {
        std::string sql = "INSERT INTO SESSION (ID, CURRENT_PATH, EDITOR_PATH, LAST_OPENED) VALUES (?, ?, ?, ?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, static_cast<int32_t>(session.id));
            query.bind(2, session.current_path);
            query.bind(3, session.editor_path);
            query.bind(4, static_cast<int32_t>(session.last_opened));

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    Entity::Session SessionRepositoryImplSqlite::get()
    {
        std::string sql = "SELECT ID, CURRENT_PATH, EDITOR_PATH, LAST_OPENED FROM SESSION WHERE ID = 1";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            if (query.executeStep())
            {
                return Entity::Session(
                    query.getColumn(0),
                    query.getColumn(1).getString(),
                    query.getColumn(2).getString(),
                    static_cast<int64_t>(query.getColumn(3))
                );
            }
            err << ("No session found") << std::endl;
            return Entity::Session(0, "", "", 0);
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    void SessionRepositoryImplSqlite::update(const Entity::Session& session)
    {
        std::string sql = "UPDATE SESSION SET CURRENT_PATH = ?, EDITOR_PATH = ?, LAST_OPENED = ? WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, session.current_path);
            query.bind(2, session.editor_path);
            query.bind(3, static_cast<int64_t>(session.last_opened));
            query.bind(4, static_cast<int32_t>(session.id));

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }
}
