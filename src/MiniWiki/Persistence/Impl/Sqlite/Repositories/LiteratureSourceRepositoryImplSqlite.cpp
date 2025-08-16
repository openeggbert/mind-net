///////////////////////////////////////////////////////////////////////////////////////////////
// bit-backup: Tool detecting bit rots in files.
// Copyright (C) 2023-2025 the original author or authors.
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

#include "MiniWiki/Persistence/Api/LiteratureSourceRepository.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Repositories/LiteratureSourceRepositoryImplSqlite.h"

#include <iostream>
#include <string>
#include <vector>

#include "MiniWiki/Global.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Tables/LiteratureSourceTable.h"
#include "SQLiteCpp/Database.h"
#include "MiniWiki/Persistence/Impl/Sqlite/SqliteFileName.h"

namespace MiniWiki::Impl::Sqlite::Repositories
{
    using std::vector;
    using Persistence::Impl::Sqlite::Tables::LiteratureSourceTable;
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    LiteratureSourceRepositoryImplSqlite::LiteratureSourceRepositoryImplSqlite() = default;

    LiteratureSourceRepositoryImplSqlite::~LiteratureSourceRepositoryImplSqlite() = default;


    void LiteratureSourceRepositoryImplSqlite::create(const Entity::LiteratureSource& literature_source)
    {
        std::string sql = "INSERT INTO " +
            std::string(LiteratureSourceTable::TABLE_NAME) +
            "(" +
            std::string(LiteratureSourceTable::TITLE) + "," +
            std::string(LiteratureSourceTable::AUTHOR) + "," +
            std::string(LiteratureSourceTable::YEAR) + "," +
            std::string(LiteratureSourceTable::PUBLICATION) + "," +
            std::string(LiteratureSourceTable::URL) + "," +
            std::string(LiteratureSourceTable::CONTENT);

        sql += ")";
        sql += " VALUES (?,?,?,?,?,?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Statement query(db, sql);
        err << sql << std::endl;
        try
        {
            int i = 0;
            query.bind(++i, literature_source.title);
            query.bind(++i, literature_source.author);
            query.bind(++i, literature_source.year);
            query.bind(++i, literature_source.publication);
            query.bind(++i, literature_source.url);
            query.bind(++i, literature_source.content);

            //
            query.exec();
            //std::cout << query.getExpandedSQL() << std::endl;
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception happened during of execution of SQLite SQL statement  " << sql << ": " << e.what()
                << " " << std::endl;
            err << "Error.";
        }
    }

    Entity::LiteratureSource extractLiteratureSourceFromResultSet(const SQLite::Statement& query)
    {
        int i = 0;
        Entity::LiteratureSource result;
        result.id = query.getColumn(i++);
        result.title = query.getColumn(i++).getString();
        result.author = query.getColumn(i++).getString();
        result.year = query.getColumn(i++);
        result.publication = query.getColumn(i++).getString();
        result.url = query.getColumn(i++).getString();
        result.content = query.getColumn(i++).getString();
        return result;
    }

    Entity::LiteratureSource LiteratureSourceRepositoryImplSqlite::read(const ushort id)
    {
        vector<Entity::LiteratureSource> result{};


        std::string sql =
            "SELECT * FROM " + std::string(LiteratureSourceTable::TABLE_NAME)
        + " WHERE " + LiteratureSourceTable::ID + " = ?";

        SQLite::Database db(SQLITE_FILE_NAME,
                            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Statement query(db, sql);

        int i = 0;

        try
        {
                query.bind(++i, id);

            while (query.executeStep())
            {
                return extractLiteratureSourceFromResultSet(query);
            }
            throw std::runtime_error("No such literature source");
        }
        catch (SQLite::Exception& e)
        {
            std::cout << e.what();
            throw std::runtime_error(e.what());
        }
    }


    vector<Entity::LiteratureSource> LiteratureSourceRepositoryImplSqlite::list(std::string& title_like)
    {
        vector<Entity::LiteratureSource> result{};


        std::string sql =
            "SELECT * FROM " + std::string(LiteratureSourceTable::TABLE_NAME);
        if (!title_like.empty())
        {
            sql = sql + " WHERE " + LiteratureSourceTable::TITLE + " like '%?%";
        };
        SQLite::Database db(SQLITE_FILE_NAME,
                            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Statement query(db, sql);

        int i = 0;

        try
        {
            if (!title_like.empty())
            {
                query.bind(++i, title_like);
            }

            while (query.executeStep())
            {
                result.push_back(extractLiteratureSourceFromResultSet(query));
            }
            return result;
        }
        catch (SQLite::Exception& e)
        {
            std::cout << e.what();
            throw std::runtime_error(e.what());
        }
    }


    void LiteratureSourceRepositoryImplSqlite::remove(const Entity::LiteratureSource& literature_source)
    {
        // StringBuilder sb = new StringBuilder();
        // sb
        //         .append("DELETE FROM ")
        //         .append(FileTable.TABLE_NAME);
        // sb.append(" WHERE ");
        //
        // sb.append(FileTable.ID);
        // sb.append("=?");
        // String sql = sb.toString();
        // //System.err.println("SQL::" + sql);
        // int i = 0;
        //
        // try (
        //         Connection connection = createConnection(); PreparedStatement stmt = connection.prepareStatement(sql);) {
        //
        //     stmt.setString(++i, file.getId());
        //
        //     //System.err.println(stmt.toString());
        //     stmt.execute();
        //
        // } catch (SQLException e) {
        //     System.out.println(e.getMessage());
        //     throw new RuntimeException(e);
        // } catch (ClassNotFoundException ex) {
        //     Logger.getLogger(FileRepositoryImplSqlite.class.getName()).log(Level.SEVERE, null, ex);
        // }
    }

    // private Connection createConnection() throws ClassNotFoundException {
    //     return sqliteConnectionFactory.createConnection();
    // }


    void LiteratureSourceRepositoryImplSqlite::update(Entity::LiteratureSource& literature_source)
    {
        // StringBuilder sb = new StringBuilder();
        // sb
        //         .append("UPDATE ")
        //         .append(FileTable.TABLE_NAME)
        //         .append(" SET ")
        //         .append(FileTable.LAST_MODIFICATION_DATE).append("=?, ")
        //         .append(FileTable.LAST_CHECK_DATE).append("=?, ")
        //         .append(FileTable.HASH_SUM_VALUE).append("=?, ")
        //         .append(FileTable.HASH_SUM_ALGORITHM).append("=?, ")
        //         .append(FileTable.SIZE).append("=?, ")
        //         .append(FileTable.LAST_CHECK_RESULT).append("=? ")
        //         .append(" WHERE ").append(FileTable.ID).append("=?");
        //
        // String sql = sb.toString();
        // //System.err.println(sql);
        // try (
        //         Connection connection = createConnection(); PreparedStatement stmt = connection.prepareStatement(sql);) {
        //     int i = 0;
        //     stmt.setString(++i, file.getLastModificationDate());
        //     stmt.setString(++i, file.getLastCheckDate());
        //     stmt.setString(++i, file.getHashSumValue());
        //     stmt.setString(++i, file.getHashSumAlgorithm());
        //     stmt.setLong(++i, file.getSize());
        //     stmt.setString(++i, file.getLastCheckResult());
        //
        //     stmt.setString(++i, file.getId());
        //
        //     int numberOfUpdatedRows = stmt.executeUpdate();
        //     //System.out.println("numberOfUpdatedRows=" + numberOfUpdatedRows);
        // } catch (SQLException e) {
        //     System.out.println(e.getMessage());
        //     throw new RuntimeException(e);
        // } catch (ClassNotFoundException ex) {
        //     Logger.getLogger(FileRepositoryImplSqlite.class.getName()).log(Level.SEVERE, null, ex);
        // }
    }


    ;
}
