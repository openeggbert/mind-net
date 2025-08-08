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

#include "NoteBox/Persistence/Api/LiteratureSourceRepository.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/LiteratureSourceRepositoryImplSqlite.h"

#include <iostream>
#include <string>
#include <vector>

#include "NoteBox/Persistence/Impl/Sqlite/Tables/LiteratureSourceTable.h"
#include "SQLiteCpp/Database.h"
#include "NoteBox/Persistence/Impl/Sqlite/SqliteFileName.h"

namespace NoteBox::Impl::Sqlite::Repositories {
    using std::vector;

        LiteratureSourceRepositoryImplSqlite::LiteratureSourceRepositoryImplSqlite() = default;

        LiteratureSourceRepositoryImplSqlite::~LiteratureSourceRepositoryImplSqlite() = default;


        void LiteratureSourceRepositoryImplSqlite::create(const Entity::LiteratureSource& literature_source){

            std::string sql = "INSERT INTO " +
                              std::string(Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::TABLE_NAME) +
                    "(" +
                    std::string(NoteBox::Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::TITLE) + "," +
                    std::string(NoteBox::Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::AUTHOR) + "," +
                    std::string(NoteBox::Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::YEAR) + "," +
                    std::string(NoteBox::Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::PUBLICATION) + "," +
                    std::string(NoteBox::Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::URL) + "," +
                    std::string(NoteBox::Persistence::Impl::Sqlite::Tables::LiteratureSourceTable::CONTENT);

            sql+= ")";
            sql+=" VALUES (?,?,?,?,?,?)";

            SQLite::Database db(NoteBox::Persistence::Impl::Sqlite::SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

            SQLite::Statement query(db, sql);
            std::cerr << sql << std::endl;
            try {

                int i = 0;
                query.bind(++i, literature_source.title);
                query.bind(++i, literature_source.author);
                query.bind(++i, literature_source.year);
                query.bind(++i, literature_source.publication);
                query.bind(++i, literature_source.url);
                query.bind(++i, literature_source.author);

                //
                query.exec();

            } catch (SQLite::Exception &e) {

                std::cerr << "Exception happened during of execution of SQLite SQL statement  " << sql << ": " << e.what() << " " << std::endl;
                std::cerr << "Error.";
            }
        }
        vector<Entity::LiteratureSource> LiteratureSourceRepositoryImplSqlite::list()  {
            return vector<Entity::LiteratureSource>{};

            //         List<FsFile> result = new ArrayList<>();
            //         StringBuilder sb = new StringBuilder();
            //         sb
            //                 .append("SELECT * FROM ")
            //                 .append(FileTable.TABLE_NAME);
            //
            //         String sql = sb.toString();
            // //        System.err.println(sql);
            //         int i = 0;
            //         ResultSet rs = null;
            //         try (
            //                 Connection connection = createConnection(); PreparedStatement stmt = connection.prepareStatement(sql);) {
            //
            //             System.err.println(stmt.toString());
            //             rs = stmt.executeQuery();
            //
            //             while (rs.next()) {
            //                 result.add(extractFileFromResultSet(rs));
            //             }
            //         } catch (SQLException e) {
            //             System.out.println(e.getMessage());
            //             throw new RuntimeException(e);
            //         } catch (ClassNotFoundException ex) {
            //             Logger.getLogger(FileRepositoryImplSqlite.class.getName()).log(Level.SEVERE, null, ex);
            //         } finally {
            //             try {
            //                 if (rs != null) {
            //                     rs.close();
            //                 }
            //             } catch (SQLException ex) {
            //                 Logger.getLogger(FileRepositoryImplSqlite.class.getName()).log(Level.SEVERE, null, ex);
            //             }
            //         }
            //         return result;
        }


        void LiteratureSourceRepositoryImplSqlite::remove(const Entity::LiteratureSource& literature_source)  {

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


        void LiteratureSourceRepositoryImplSqlite::update(Entity::LiteratureSource& literature_source)  {

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

    // private: Entity::FsFile extractFileFromResultSet(const ResultSet rs) {
    //     return new Entity::FsFile(
    //             rs.getString(FileTable.ID),
    //             rs.getString(FileTable.NAME),
    //             rs.getString(FileTable.ABSOLUTE_PATH),
    //             rs.getString(FileTable.LAST_MODIFICATION_DATE),
    //             rs.getString(FileTable.LAST_CHECK_DATE),
    //             rs.getString(FileTable.HASH_SUM_VALUE),
    //             rs.getString(FileTable.HASH_SUM_ALGORITHM),
    //             rs.getLong(FileTable.SIZE),
    //             rs.getString(FileTable.LAST_CHECK_RESULT)
    //     );
    // }


    ;
}
