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

    void set_foreign_key_pragma(SQLite::Database& db)
    {
        db.exec("PRAGMA foreign_keys = ON;");
    }
    void set_temp_store_pragma(SQLite::Database& db) {
        db.exec("PRAGMA temp_store = MEMORY;");
    }

    int create_model(const entity_fields& fields, const models::ModelDefinition& definition)
    {
        std::string sql = Utils::generate_insert_sql(definition);
        std::cout << "Going to execute insert SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_foreign_key_pragma(db);
        SQLite::Statement query(db, sql);

        Utils::fill_sqlite_query(query, fields
                                 , definition.auto_increment
        );

        Utils::sqlite_exec(query);
        return db.getLastInsertRowid();
    }

    entity_fields read_model(models::ModelDefinition& def, const int id)
    {
        std::string sql = Utils::generate_select_one_sql(def.model_name);
        std::cout << "Going to execute select one SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_foreign_key_pragma(db);
        set_temp_store_pragma(db);
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

    bool update_model(int id, models::ModelDefinition& def, entity_fields& fields)
    {
        std::string sql = Utils::generate_update_sql(def);
        std::cout << "Going to execute update SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_foreign_key_pragma(db);
        set_temp_store_pragma(db);
        SQLite::Statement query(db, sql);

        if (!fields.empty())
        {
            fields.erase(fields.begin());
        }
        fields.push_back(id);

        Utils::fill_sqlite_query(query, fields, false);
        try
        {
            Utils::sqlite_exec(query);
            std::cout << "Update successful" << std::endl;
            return true;
        }
        catch (std::exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            return false;
        }
    }

    bool delete_model(models::ModelDefinition& def, const int id)
    {
        str sql = Utils::generate_delete_sql(def);
        std::cout << "Going to execute delete SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_foreign_key_pragma(db);
        set_temp_store_pragma(db);
        SQLite::Statement query(db, sql);


        entity_fields fields;
        fields.push_back(id);
        Utils::fill_sqlite_query(query, fields, false);
        try
        {
            Utils::sqlite_exec(query);
            std::cout << "Delete successful" << std::endl;
            return true;
        }
        catch (std::exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            return false;
        }
    }

    std::vector<entity_fields> list_models(models::ModelDefinition& def, size_t page_number, size_t page_size)
    {
        std::string sql = Utils::generate_select_all_sql(def.model_name);
        std::cout << "Going to execute select all SQL: " << sql << std::endl;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_foreign_key_pragma(db);
        SQLite::Statement query(db, sql);
        std::cout << "Page size: " << page_size << std::endl;
        std::cout << "Page number: " << page_number << std::endl;
        query.bind(1, static_cast<int32_t>(page_size));
        query.bind(2, static_cast<int32_t>(page_size * (page_number - 1)));

        std::vector<entity_fields> results;
        while (query.executeStep())
        {
            entity_fields result;
            int i = 0;
            for (const auto& column : def.columns)
            {
                //std::cout << "Found entity with id: " << query.getColumn(1) << std::endl;
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
            results.push_back(result);
        }
        return results;
    }
}

#endif // BASEREPOSITORY_H
