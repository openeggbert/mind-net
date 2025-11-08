//
// Created by robertvokac on 9/23/25.
//

#include "mindnet/orm/SqlUtils.h"

#include "mindnet/model/ModelDefinition.h"
#include "mindnet/orm/Order.h"
#include "mindnet/orm/QueryParams.h"

namespace mindnet::orm
{
    string SqlUtils::generate_insert_sql(const model::ModelDefinition& definition)
    // const std::string& table_name, const std::vector<const char*>& columns,
    //                                    bool auto_increment)
    {
        string sql = "INSERT INTO " + definition.get_model_name() + " (";
        const auto& columns = definition.get_columns();
        for (int i = 0; i < columns.size(); ++i)
        {
            auto& column = columns[i].get_column_name();
            if (std::string(column) == PRIMARY_KEY_COLUMN_NAME)
            {
                continue;
            }
            sql += column;
            if (i < columns.size() - 1)
            {
                sql += ", ";
            }
        }
        sql += ") VALUES (";
        for (int i = 0; i < columns.size(); ++i)
        {
            auto column = columns[i].get_column_name();
            if (std::string(column) == PRIMARY_KEY_COLUMN_NAME)
            {
                continue;
            }
            sql += "?";
            if (i < columns.size() - 1)
            {
                sql += ", ";
            }
        }
        sql += ")";
        return sql;
    }

    string SqlUtils::generate_update_sql(const model::ModelDefinition& definition)
    {
        std::string sql = "UPDATE " + definition.get_model_name() + " SET ";
        for (int i = 0; i < definition.get_columns().size(); ++i)
        {
            auto column = definition.get_columns()[i].get_column_name();
            if (std::string(column) == PRIMARY_KEY_COLUMN_NAME)
            {
                continue;
            }
            if (std::string(column) == model::BaseColumns::CREATED_AT)
            {
                //created_at won't be changed in any case
                continue;
            }
            sql += column + "=?";
            if (i < definition.get_columns().size() - 1)
            {
                sql += ", ";
            }
        }

        sql += " WHERE ID = ?";
        return sql;
    }

    string SqlUtils::generate_delete_sql(const model::ModelDefinition& definition)
    {
        return "DELETE FROM " + definition.get_model_name() + " WHERE id = ?";
    }

    string SqlUtils::generate_select_one_sql(const std::string& table_name, const model::ModelDefinition& definition)
    {
        std::string sql = "SELECT ";
        auto& columns = definition.get_columns();
        int column_index = 0;
        int last_column = columns.size() - 1;
        for (auto& col:columns)
        {
            sql+= col.get_column_name();
            sql += column_index < last_column ? ", " : " ";
            column_index++;
        }
        sql+=  "FROM " + table_name + " WHERE id = ?";
        return sql;
    }

    /**
     * Generates a SQL query to select all rows from a specified table, with optional filtering, sorting, and pagination.
     *
     * @param table_name The name of the table to query.
     * @param query_params An object containing the filtering, sorting, and pagination parameters for the query.
     *                     - `filters`: A map of column-value pairs used as filters in the WHERE clause.
     *                     - `sort`: An optional field to specify the column for sorting.
     *                     - `order`: An optional field to specify the sort order (e.g., ASC or DESC).
     * @param count A boolean flag. If true, modifies the query to ignore sorting and pagination, suitable for row count queries. Default is false.
     * @return The constructed SQL query as a string.
     */
    string SqlUtils::generate_select_all_sql(
        const std::string& table_name,
        const orm::QueryParams& query_params,
        model::ModelDefinition& def,
        bool count)
    {
        auto sql = count ? ("SELECT count(*) as c FROM " + table_name) : ("SELECT * FROM " + table_name);
        if (!query_params.filters.empty())
        {
            auto filter = query_params.filters;
            sql += " WHERE ";
            for (auto it = filter.begin(); it != filter.end(); ++it)
            {
                const auto& column_name = it->first;
                const auto& value = it->second;
                bool foreign_key = false;
                for (auto& col : def.get_columns())
                {
                    if (std::string(col.get_column_name()) == column_name)
                    {
                        foreign_key = col.is_foreign_key();
                        break;
                    }
                }
                if (foreign_key && value == "0")
                {
                    sql += column_name + " IS NULL ";
                }
                else
                {
                    sql += column_name + " = ?";
                }

                if (std::next(it) != filter.end())
                {
                    sql += " AND ";
                }
            }
        }

        if (!count && query_params.sort.has_value() && !query_params.sort.value().empty())
        {
            sql += " ORDER BY " + query_params.sort.value() + " ";
            if (query_params.order.has_value())
            {
                sql += order_to_string(query_params.order.value());
            }
        }

        if (!count) { sql += " LIMIT ? OFFSET ?"; };
        return sql;
    }

    string SqlUtils::generate_select_count_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                               model::ModelDefinition& def)
    {
        return generate_select_all_sql(table_name, query_params, def, true);
    }
}
