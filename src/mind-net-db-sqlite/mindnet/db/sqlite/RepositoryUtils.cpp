///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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

#include <vector>

#include "mindnet/util/Utils.h"
#include "mindnet/essential/Global.h"
#include "mindnet/orm/QueryParams.h"
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "SQLiteCpp/Database.h"
#include "mindnet/model/ModelDefinition.h"
#include "mindnet/orm/SqlUtils.h"

namespace mindnet::db::sqlite
{
    using std::vector;
    using sqlite::SQLITE_FILE_NAME;
    using util::Utils;

    using_loggers()

    void sqlite_exec(SQLite::Statement& query)
    {
        try
        {
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << commit;
            throw std::runtime_error(e.what());
        }
    }

    template <class>
    inline constexpr bool always_false = false;

    void fill_sqlite_query(
        SQLite::Statement& query,
        const entity_fields& values,
        const model::ModelDefinition& model_definition,
        const essential::Crudl crudl,
        bool auto_increment = false)
    {
        std::size_t values_size = values.size();
        if (auto_increment) { values_size--; }
        if (values_size > static_cast<size_t>(query.getBindParameterCount()))
        {
            err << "Expected: " << query.getBindParameterCount() << " values, got: " << values_size << std::endl;
            err << "values().size " << values_size << " > query.getBindParameterCount() " << static_cast<size_t>(query.
                getBindParameterCount()) << std::endl;
            throw std::out_of_range("More values provided than query parameters");
        }
        if (values_size < static_cast<size_t>(query.getBindParameterCount()))
        {
            err << "Expected: " << query.getBindParameterCount() << " values, got: " << values_size << std::endl;
            err << "values().size " << values_size << " < query.getBindParameterCount() " << static_cast<size_t>(query.
                getBindParameterCount()) << std::endl;
            throw std::out_of_range("Less values provided than query parameters");
        }

        auto& columns = model_definition.get_columns();
        for (size_t i = 0; i < values.size(); ++i)
        {
            if (auto_increment && i == 0)
            {
                continue;
            }
            string column_name;
            bool is_foreign_key = false;
            bool create = crudl == essential::Crudl::Create;
            bool update = crudl == essential::Crudl::Update;

            if (create || update)
            {
                bool update_id = update && i == (values.size() - 1);
                if (update_id)
                {
                    column_name = model::BaseColumns::ID;
                }
                else
                {
                    auto& column = columns[i + (update ? 2 : 0)];
                    column_name = column.get_column_name();
                    is_foreign_key = column.is_foreign_key();
                }
            }

            if (update && i < (values.size() - 1))
            {
                auto& column = columns[i + 2];
                column_name = column.get_column_name();
                is_foreign_key = column.is_foreign_key();
            }

            std::visit([&](auto&& val) -> void
            {
                using T = std::decay_t<decltype(val)>;

                int index = static_cast<int>(i + 1 + (auto_increment ? -1 : 0));
                if constexpr (std::is_same_v<T, std::string>)
                {
                    trace << "binding index " << i << " " << column_name << " with value: \"" << val << "\"" << commit;
                    query.bind(index, val);
                }
                else if constexpr (std::is_same_v<T, int64_t>)
                {
                    if (val == 0 && is_foreign_key)
                    {
                        trace << "binding index " << i << " " << column_name << " with value: NULL" << commit;
                        query.bind(index, nullptr);
                    }
                    else
                    {
                        trace << "binding index " << i << " " << column_name << " with value: " << val << commit;
                        query.bind(index, val);
                    }
                }
                else
                {
                    static_assert(always_false<T>, "Unsupported type in SqlValue");
                }
            }, values[i]);
        }
    }

    void set_pragmas(SQLite::Database& db, bool temp_store = false)
    {
        db.exec("PRAGMA foreign_keys = ON;");
        db.exec("PRAGMA journal_mode = WAL;");
        db.exec("PRAGMA busy_timeout = 5000;");
        if (temp_store)
        {
            db.exec("PRAGMA temp_store = MEMORY;");
        }
    }

    int create_model(const entity_fields& fields, const model::ModelDefinition& definition, string& error)
    {
        std::string sql = orm::SqlUtils::generate_insert_sql(definition);
        debug << "Going to execute insert SQL: " << sql << commit;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_pragmas(db);

        SQLite::Statement* query_ptr = nullptr;

        try
        {
            query_ptr = new SQLite::Statement(db, sql);
        }
        catch (const SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return -1;
        }

        fill_sqlite_query(*query_ptr, fields, definition, essential::Crudl::Create, true
        );

        try
        {
            sqlite_exec(*query_ptr);
        }
        catch (std::exception& e)
        {
            error = e.what();
            delete query_ptr;
            throw;
        }
        delete query_ptr;
        return db.getLastInsertRowid();
    }

    entity_fields read_model(model::ModelDefinition& def, const int id, string& error)
    {
        std::string sql = orm::SqlUtils::generate_select_one_sql(def.get_model_name());
        debug << "Going to execute select one SQL: " << sql << commit;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_pragmas(db, true);

        SQLite::Statement* query_ptr = nullptr;

        try
        {
            query_ptr = new SQLite::Statement(db, sql);
        }
        catch (const SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return {};
        }

        entity_fields result;
        auto columns = def.get_columns();
        (*query_ptr).bind(1, id);

        if ((*query_ptr).executeStep())
        {
            int i = 0;
            for (const auto& column : def.get_columns())
            {
                auto primitive_column_type = model::column_type_to_primitive_column_type(column.get_column_type());
                switch (primitive_column_type)
                {
                case model::PrimitiveColumnType::Text:
                    {
                        // if (column.is_hidden())
                        // {
                        //     result.push_back("*");
                        // } else
                        // {
                        string text = (*query_ptr).getColumn(i).getString();
                        result.push_back(text);
                        // }
                    }
                    break;
                case model::PrimitiveColumnType::Number:
                    {
                        // if (column.is_hidden())
                        // {
                        //     result.push_back(cast64(0));
                        // }
                        // else
                        // {
                        int number = (*query_ptr).getColumn(i);
                        result.push_back(number);
                        // }
                    }
                    break;
                default:
                    {
                        error = "Unknown type";
                        throw std::runtime_error("Unknown type");
                    }
                }
                i++;
            }
            delete query_ptr;
            return result;
        }

        delete query_ptr;

        error = def.get_model_name() + " not found";
        return {};
    }

    bool update_model(int id, model::ModelDefinition& def, entity_fields& fields_, string& error)
    {
        std::string sql = orm::SqlUtils::generate_update_sql(def);
        debug << "Going to execute update SQL: " << sql << commit;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_pragmas(db, true);

        SQLite::Statement* query_ptr = nullptr;

        try
        {
            query_ptr = new SQLite::Statement(db, sql);
        }
        catch (const SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return false;
        }

        entity_fields fields_copy = fields_;
        fields_copy.erase(fields_copy.begin(),
                          fields_copy.begin() + std::min<size_t>(2, fields_copy.size()));

        fields_copy.push_back(id);

        fill_sqlite_query(*query_ptr, fields_copy, def, essential::Crudl::Update);
        try
        {
            sqlite_exec(*query_ptr);
            debug << "Update successful" << commit;
            delete query_ptr;
            return true;
        }
        catch (std::exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << commit;
            error = e.what();
            delete query_ptr;
            return false;
        }
    }

    bool delete_model(model::ModelDefinition& def, const int id, string& error)
    {
        string sql = orm::SqlUtils::generate_delete_sql(def);
        debug << "Going to execute delete SQL: " << sql << commit;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_pragmas(db, true);
        SQLite::Statement* query_ptr = nullptr;

        try
        {
            query_ptr = new SQLite::Statement(db, sql);
        }
        catch (const SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return false;
        }

        entity_fields fields;
        fields.push_back(id);
        fill_sqlite_query(*query_ptr, fields, def, essential::Crudl::Delete);
        try
        {
            sqlite_exec(*query_ptr);
            auto number_of_deleted_rows = db.getChanges();
            if (number_of_deleted_rows != 1)
            {
                err << "Expected to delete 1 row, but deleted " << number_of_deleted_rows << std::endl;
                error = std::string("Expected to delete 1 row, but deleted ") + std::to_string(number_of_deleted_rows);
                delete query_ptr;
                return false;
            }
            debug << "Delete successful" << std::endl;
            delete query_ptr;
            return true;
        }
        catch (std::exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << commit;
            error = e.what();
            delete query_ptr;
            return false;
        }
    }

    void bind_query_filters(
        model::ModelDefinition& def,
        orm::QueryParams& query_params,
        SQLite::Statement& query,
        int& bind_index)
    {
        if (!query_params.filters.empty())
        {
            for (auto& filter : query_params.filters)
            {
                auto key = filter.first;
                auto value = filter.second;
                mindnet::model::ColumnType column_type{mindnet::model::ColumnType::Text};
                bool foreign_key = false;
                bool column_type_found = false;
                for (auto& column : def.get_columns())
                {
                    if (column.get_column_name() == key)
                    {
                        column_type = column.get_column_type();
                        column_type_found = true;
                        if (column.is_foreign_key())
                        {
                            foreign_key = true;
                        }
                        break;
                    }
                }
                if (!column_type_found)
                {
                    err << "Filter column " << key << " not found in model " << def.get_model_name() << std::endl;
                    throw std::runtime_error("Filter column not found: " + key);
                }
                if (foreign_key && model::column_type_to_primitive_column_type(column_type)==model::PrimitiveColumnType::Number && stoi(value) == 0)
                {
                    continue;
                }
                switch (column_type)
                {
                case mindnet::model::ColumnType::TextArea:
                case mindnet::model::ColumnType::Text:
                    debug << "Binding index " << bind_index << " with value " << value << commit;
                    query.bind(bind_index++, value);

                    break;
                case mindnet::model::ColumnType::Bool:
                case mindnet::model::ColumnType::DateTime:
                case mindnet::model::ColumnType::Integer:
                    {
                        int number = stoi(value);
                        // if (foreign_key && number == 0)
                        // {
                        //     debug << "Binding index " << bind_index << " with value NULL" << commit;
                        //     query.bind(bind_index++);
                        // }
                        // else
                        // {
                            debug << "Binding index " << bind_index << " with value " << value << commit;
                            query.bind(bind_index++, number);
                        // }
                    }
                    break;
                default: throw std::runtime_error("Unknown type " + column_type_to_string(column_type));
                }
            }
        }
    }

    std::vector<entity_fields> list_models(
        model::ModelDefinition& def,
        orm::QueryParams& query_params,
        string& error
    )
    {
        trace << "list_models()" << commit;
        std::string sql = orm::SqlUtils::generate_select_all_sql(def.get_model_name(), query_params, def);
        std::string sql_count = orm::SqlUtils::generate_select_count_sql(def.get_model_name(), query_params, def);

        debug << "Going to execute select all SQL: " << sql << commit;

        SQLite::Database db(
            SQLITE_FILE_NAME,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        set_pragmas(db);

        SQLite::Statement* query_ptr = nullptr;

        try
        {
            query_ptr = new SQLite::Statement(db, sql);
        }
        catch (const SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return {};
        }

        try
        {
            SQLite::Statement query(db, sql);
        }
        catch (SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return {};
        }

        debug << "Page size: " << query_params.page_size << commit;
        debug << "Page number: " << query_params.page_number << commit;
        int bind_index = 1;
        bind_query_filters(def, query_params, *query_ptr, bind_index);
        // if (query_params.sort.has_value())
        // {
        //     (*query_ptr).bind(bind_index++, query_params.sort.value());
        //     if (query_params.order.has_value())
        //     {
        //         (*query_ptr).bind(bind_index++, order_to_string(query_params.order.value()));
        //     }
        // }
        experiment << "Binding index " << bind_index << " with value " + std::to_string(query_params.page_size) <<
            commit;
        (*query_ptr).bind(bind_index++, static_cast<int32_t>(query_params.page_size));
        experiment << "Binding index " << bind_index << " with value " + std::to_string(
            query_params.page_size * (query_params.page_number - 1)) << commit;
        (*query_ptr).bind(bind_index++, static_cast<int32_t>(query_params.page_size * (query_params.page_number - 1)));

        std::vector<entity_fields> results;
        try
        {
            while ((*query_ptr).executeStep())
            {
                entity_fields result;
                int i = 0;
                for (const auto& column : def.get_columns())
                {
                    switch (column.get_column_type())
                    {
                    case mindnet::model::ColumnType::TextArea:
                    case mindnet::model::ColumnType::Text:
                        {
                            string text = (*query_ptr).getColumn(i).getString();
                            result.push_back(text);
                        }
                        break;
                    case mindnet::model::ColumnType::Bool:
                    case mindnet::model::ColumnType::DateTime:
                    case mindnet::model::ColumnType::Integer:
                        {
                            int number = (*query_ptr).getColumn(i);
                            result.push_back(number);
                        }
                        break;
                    default: throw std::runtime_error("Unknown type");
                    }
                    i++;
                }
                results.push_back(result);
            }
        }
        catch (SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return results;
        }

        SQLite::Statement query_count(db, sql_count);
        bind_index = 1;
        bind_query_filters(def, query_params, query_count, bind_index);

        try
        {
            while (query_count.executeStep())
            {
                query_params.total_items = static_cast<int>(query_count.getColumn(0));
                break;
            }
        }
        catch (SQLite::Exception& e)
        {
            error = e.what();
            delete query_ptr;
            return results;
        }
        delete query_ptr;
        return results;
    }
}
