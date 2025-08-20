// ///////////////////////////////////////////////////////////////////////////////////////////////
// // mini-wiki: Lightweight wiki inspired by MediaWiki.
// // Copyright (C) 2025-2025 the original author or authors.
// //
// // This program is free software: you can redistribute it and/or
// // modify it under the terms of the GNU General Public License
// // as published by the Free Software Foundation, either version 3
// // of the License, or (at your option) any later version.
// //
// // This program is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// // GNU General Public License for more details.
// //
// // You should have received a copy of the GNU General Public License
// // along with this program. If not, see
// // <https://www.gnu.org/licenses/> or write to the Free Software
// // Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// ///////////////////////////////////////////////////////////////////////////////////////////////
//
// /**
//  *
//  * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
//  */
//
// #include "mindnet/persistence/impl/sqlite/repositories/IdeaRepositoryImplSqlite.h"
// #include "mindnet/models/Idea.h"
//
// #include <iostream>
// #include <string>
// #include <vector>
//
// #include "mindnet/Global.h"
// #include "SQLiteCpp/Database.h"
// #include "mindnet/persistence/impl/sqlite/SqliteFileName.h"
// #include "mindnet/models/columns/IdeaTable.h"
//
// namespace mindnet::impl::sqlite::repositories
// {
//
//     using mindnet::models::columns::IdeaTable;
//
//     IdeaRepositoryImplSqlite::IdeaRepositoryImplSqlite() = default;
//
//     IdeaRepositoryImplSqlite::~IdeaRepositoryImplSqlite() = default;
//
//
//
//     void IdeaRepositoryImplSqlite::create(const Entity::Idea& idea)
//     {
//         create_entity(idea);
//     }
//
//     Entity::Idea IdeaRepositoryImplSqlite::read(int id)
//     {
//         std::string sql = "SELECT ID, TEXT, CREATED_AT, DUE_AT FROM IDEA WHERE ID = ?";
//
//         SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
//         SQLite::Statement query(db, sql);
//
//         try
//         {
//             query.bind(1, id);
//
//             if (query.executeStep())
//             {
//                 Entity::Idea idea{};
//                 idea.id = query.getColumn(0).getInt();
//                 idea.text = query.getColumn(1).getString();
//                 idea.created_at = query.getColumn(2).getInt64();
//                 idea.due_date = query.getColumn(3).getInt64();
//
//                 return idea;
//             }
//             throw std::runtime_error("Idea not found");
//         }
//         catch (SQLite::Exception& e)
//         {
//             err << "Exception during SQLite statement execution: " << e.what() << std::endl;
//             throw std::runtime_error(e.what());
//         }
//     }
//
//     void IdeaRepositoryImplSqlite::update(const Entity::Idea& idea)
//     {
//         std::string sql = "UPDATE IDEA SET TEXT = ?, CREATED_AT = ?, DUE_AT = ? WHERE ID = ?";
//
//         SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
//         SQLite::Statement query(db, sql);
//
//         try
//         {
//             query.bind(1, idea.text);
//             query.bind(2, static_cast<int64_t>(idea.created_at));
//             query.bind(3, static_cast<int64_t>(idea.due_date));
//             query.bind(4, idea.id);
//
//             query.exec();
//         }
//         catch (SQLite::Exception& e)
//         {
//             err << "Exception during SQLite statement execution: " << e.what() << std::endl;
//             throw std::runtime_error(e.what());
//         }
//     }
//
//     void IdeaRepositoryImplSqlite::remove(int id)
//     {
//         std::string sql = "DELETE FROM IDEA WHERE ID = ?";
//
//         SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
//         SQLite::Statement query(db, sql);
//
//         try
//         {
//             query.bind(1, id);
//             query.exec();
//         }
//         catch (SQLite::Exception& e)
//         {
//             err << "Exception during SQLite statement execution: " << e.what() << std::endl;
//             throw std::runtime_error(e.what());
//         }
//     }
//
//     std::vector<Entity::Idea> IdeaRepositoryImplSqlite::list(str& category, size_t pageNumber, size_t pageSize)
//     {
//         std::string sql = "SELECT ID, TEXT, CREATED_AT, DUE_AT FROM IDEA LIMIT ? OFFSET ?";
//
//         SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
//         SQLite::Statement query(db, sql);
//
//         try
//         {
//             query.bind(1, static_cast<int32_t>(pageSize));
//             query.bind(2, static_cast<int32_t>(pageSize * pageNumber));
//
//             std::vector<Entity::Idea> ideas;
//
//             while (query.executeStep())
//             {
//                 Entity::Idea idea{};
//                 idea.id = query.getColumn(0).getInt();
//                 idea.text = query.getColumn(1).getString();
//                 idea.created_at = query.getColumn(2).getInt64();
//                 idea.due_date = query.getColumn(3).getInt64();
//
//                 ideas.push_back(idea);
//             }
//
//             return ideas;
//         }
//         catch (SQLite::Exception& e)
//         {
//             err << "Exception during SQLite statement execution: " << e.what() << std::endl;
//             throw std::runtime_error(e.what());
//         }
//     }
// }
