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

#include "MiniWiki/Persistence/Impl/Sqlite/Repositories/TermRepositoryImplSqlite.h"
#include "MiniWiki/Entity/Term.h"

#include <iostream>
#include <string>
#include <vector>

#include "MiniWiki/Global.h"
#include "SQLiteCpp/Database.h"
#include "MiniWiki/Persistence/Impl/Sqlite/SqliteFileName.h"

namespace MiniWiki::Impl::Sqlite::Repositories
{
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    TermRepositoryImplSqlite::TermRepositoryImplSqlite() = default;

    TermRepositoryImplSqlite::~TermRepositoryImplSqlite() = default;

    void TermRepositoryImplSqlite::create(const Entity::Term& term)
    {
        std::string sql = "INSERT INTO TERM (NAME, NOTE_ID, CATEGORY) "
            "VALUES (?, ?, ?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, term.name);
            query.bind(2, term.note_id);
            query.bind(3, term.category);

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    Entity::Term TermRepositoryImplSqlite::read(const std::string& id)
    {
        std::string sql = "SELECT ID, NAME, NOTE_ID, CATEGORY "
            "FROM TERM WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, id);

            if (query.executeStep())
            {
                Entity::Term term{};
                term.id = query.getColumn(0).getString();
                term.name = query.getColumn(1).getString();
                term.note_id = query.getColumn(2).getString();
                term.category = query.getColumn(3).getString();

                return term;
            }
            throw std::runtime_error("read() Term not found");
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    void TermRepositoryImplSqlite::update(const Entity::Term& term)
    {
        std::string sql = "UPDATE TERM SET NAME = ?, NOTE_ID = ?, CATEGORY = ? "
            " WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, term.name);
            query.bind(2, term.note_id);
            query.bind(3, term.category);
            query.bind(4, term.id);

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    str TermRepositoryImplSqlite::id_for_name_and_category(str& name, str& category)
    {
        std::string sql = "SELECT ID "
       "FROM TERM WHERE NAME=? AND CATEGORY=?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 1;
            query.bind(i++, name);
            query.bind(i++, category);

            std::vector<Entity::Term> terms;

            while (query.executeStep())
            {
                return query.getColumn(0).getString();
            }

            err << "Term not found" << std::endl;
            throw std::runtime_error("id_for_name_and_category() Term not found");
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    void TermRepositoryImplSqlite::remove(const std::string& id)
    {
        std::string sql = "DELETE FROM TERM WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, id);
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::vector<str> TermRepositoryImplSqlite::list_categories()
    {
        std::string sql = "SELECT DISTINCT CATEGORY "
    "FROM TERM";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {

            std::vector<str> categories;

            while (query.executeStep())
            {
                categories.push_back(query.getColumn(0).getString());
            }

            return categories;
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::vector<Entity::Term> TermRepositoryImplSqlite::list(std::string& category, size_t pageNumber,
                                                             size_t pageSize)
    {
        std::string sql = std::string("SELECT ID, NAME, NOTE_ID, CATEGORY ") +
            "FROM TERM " +
                (category.empty() ? "" : "WHERE CATEGORY=? ") +
                    "LIMIT ? OFFSET ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            int i = 1;
            if (!category.empty()){query.bind(i++, category);}
            query.bind(i++, static_cast<int32_t>(pageSize));
            query.bind(i++, static_cast<int32_t>(pageSize * pageNumber));

            std::vector<Entity::Term> terms;

            while (query.executeStep())
            {
                Entity::Term term{};
                term.id = query.getColumn(0).getString();
                term.name = query.getColumn(1).getString();
                term.note_id = query.getColumn(2).getString();
                term.category = query.getColumn(3).getString();

                terms.push_back(term);
            }

            return terms;
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }
}
