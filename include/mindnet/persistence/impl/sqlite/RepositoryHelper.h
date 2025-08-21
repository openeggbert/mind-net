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
#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <iostream>

#include "mindnet/models/Content.h"
#include <vector>

#include "mindnet/Utils.h"
#include "mindnet/persistence/impl/sqlite/SqliteFileName.h"
#include "SQLiteCpp/Database.h"

namespace mindnet::persistence::impl::sqlite
{
    using std::vector;
    using models::columns::ContentColumns;
    using sqlite::SQLITE_FILE_NAME;

    inline int create_model(const entity_fields& fields, const models::ModelDefinition& definition)
    {

        std::string sql = Utils::generate_insert_sql(definition);
        std::cout << "Going to execute SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        SQLite::Statement query(db, sql);

        Utils::fill_sqlite_query(query, fields
                                 , definition.auto_increment
        );

        Utils::sqlite_exec(query);
        return db.getLastInsertRowid();
    }

    inline entity_fields read_model(models::ModelDefinition& def, const int id)
    {
        std::string sql = Utils::generate_select_one_sql(def.model_name);
        std::cout << "Going to execute SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        SQLite::Statement query(db, sql);

        entity_fields result;
        auto columns = def.columns;
        query.bind(1, id);


        if (query.executeStep())
        {
            int i = 0;
            for (const auto& column : def.columns)
            {
                switch (column.column_type)
                {
                case enums::ColumnType::TEXT:
                    {
                        str text = query.getColumn(i).getString();
                        result.push_back(text);
                    }
                    break;
                case enums::ColumnType::INTEGER:
                    {
                        int number = query.getColumn(i);
                        result.push_back(number);
                    }
                    break;
                default: throw std::runtime_error("Unknown type");

                }
                i++;
            }
            return result;
        }


        throw std::runtime_error(def.model_name + " not found");
    }
}

#endif // BASEREPOSITORY_H
