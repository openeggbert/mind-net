//
// Created by robertvokac on 9/23/25.
//

#include "mindnet/orm/OrmUtils.h"

#include "mindnet/model/ModelDefinition.h"
#include "mindnet/orm/Order.h"
#include "mindnet/orm/QueryParams.h"

namespace mindnet::orm
{

    string OrmUtils::generate_insert_sql(const model::ModelDefinition& definition)
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

    string OrmUtils::generate_update_sql(const model::ModelDefinition& definition)
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

    string OrmUtils::generate_delete_sql(const model::ModelDefinition& definition)
    {
        return "DELETE FROM " + definition.get_model_name() + " WHERE id = ?";
    }

    string OrmUtils::generate_select_one_sql(const std::string& table_name)
    {
        return "SELECT * FROM " + table_name + " WHERE id = ?";
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
    string OrmUtils::generate_select_all_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                          bool count)
    {
        auto sql = count ? ("SELECT count(*) as c FROM " + table_name) : ("SELECT * FROM " + table_name);
        if (!query_params.filters.empty())
        {
            auto filter = query_params.filters;
            sql += " WHERE ";
            int filter_count = filter.size();
            int filter_index = 1;
            for (auto& filter_item : filter)
            {
                sql += filter_item.first + " = ?";
                filter_index++;
                if (filter_index < filter_count)
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

    string OrmUtils::generate_select_count_sql(const std::string& table_name, const orm::QueryParams& query_params)
    {
        return generate_select_all_sql(table_name, query_params, true);
    }

}
