///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Lightweight wiki inspired by MediaWiki.
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
#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <iostream>

#include "miniwiki/models/Content.h"
#include <vector>

#include "miniwiki/Utils.h"
#include "miniwiki/persistence/impl/sqlite/SqliteFileName.h"
#include "SQLiteCpp/Database.h"

namespace miniwiki::persistence::impl::sqlite {

    using std::vector;
    using models::columns::ContentColumns;
    using sqlite::SQLITE_FILE_NAME;

    inline int create_model(const models::BaseModel& entity)
    {
        auto definition = entity.get_definition();
        std::string sql = Utils::generate_insert_sql(entity);
        std::cout << "Going to execute SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
            );
        SQLite::Statement query(db, sql);

        Utils::fill_sqlite_query(query, entity.get_values()
            , definition.auto_increment
    );

        Utils::sqlite_exec(query);
        return db.getLastInsertRowid();

    }

    inline entity_fields read_model(str& model_name, const int id)
    {
        entity_fields result;
        return result;

    //     std::string sql = Utils::generate_select_one_sql(model_name);
    //     std::cout << "Going to execute SQL: " << sql << std::endl;
    //
    //     SQLite::Database db(
    //         SQLITE_FILE_NAME,
    //         SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
    //     );
    //     SQLite::Statement query(db, sql);
    //
    //     entity_fields result;
    //     auto columns = entity.get_entity_columns();
    //
    //
    //
    //
    //
    //     Utils::fill_sqlite_query(query, entity.get_entity_fields()
    //         , entity.should_be_id_auto_incremented()
    // );
    //
    //     Utils::sqlite_exec(query);

    }



}

#endif // BASEREPOSITORY_H
