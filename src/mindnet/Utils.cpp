//
// Created by robertvokac on 7/31/25.
//

#include "mindnet/Utils.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <random>
#include <stdexcept>
#include <regex>

#include "mindnet/Global.h"
#include "mindnet/http/QueryParams.h"
#include "SQLiteCpp/Database.h"

namespace mindnet
{
    SM2Result Utils::calculate_sm2(int quality, int current_repetition, int current_interval, double current_ef)
    {
        SM2Result updated{};

        if (quality < 3)
        {
            updated.new_repetition = 0;
            updated.new_interval = 1;
        }
        else
        {
            updated.new_repetition = current_repetition + 1;
            if (current_repetition == 0)
            {
                updated.new_interval = 1;
            }
            else if (current_repetition == 1)
            {
                updated.new_interval = 6;
            }
            else
            {
                updated.new_interval = static_cast<int>(current_interval * current_ef);
            }
        }

        // Update of EF based on the quality
        double new_ef = current_ef + (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
        if (new_ef < 1.3) new_ef = 1.3;

        updated.new_ef = new_ef;
        return updated;

        //example auto result = calculate_sm2(4, 2, 10, 2.5);
        // result.new_interval = 25
        // result.next_ef = 2.46
        // result.new_repetition = 3
    }

    long long Utils::currentTimestamp()
    {
        time_t now = time(nullptr);
        struct tm* now2 = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", now2);
        return atoll(buffer);
    }

    long long Utils::currentUnixTimestamp()
    {
        return static_cast<long long>(std::time(nullptr));
    }

    std::string Utils::unixToFormattedString(ll unixTimestamp)
    {
        if (unixTimestamp == 0)
        {
            return "none";
        }
        std::time_t rawTime = static_cast<std::time_t>(unixTimestamp);
        std::tm* timeInfo = std::localtime(&rawTime);

        char buffer[21];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        return std::string(buffer);
    }

    string Utils::print_current_timestamp()
    {
        return Utils::unixToFormattedString(Utils::currentUnixTimestamp());
    }

    // Convert single letter 'a'-'z' to number 0-25
    int Utils::letterToDecimal(char letter)
    {
        if (letter < 'a' || letter > 'z')
        {
            throw std::invalid_argument("letterToDecimal: input must be a-z");
        }
        return letter - 'a';
    }

    // Convert number 0-25 to single letter 'a'-'z'
    char Utils::decimalToLetter(int number)
    {
        if (number < 0 || number > 25)
        {
            throw std::out_of_range("decimalToLetter: input must be 0-25");
        }
        return static_cast<char>('a' + number);
    }

    // Convert base-26 string (a-z) to decimal integer
    // "a" -> 0, "b" -> 1, ..., "z" -> 25, "aa" -> 26, etc.
    int Utils::base26ToDecimal(const std::string& text)
    {
        if (text.empty())
            throw std::invalid_argument("Empty string not allowed");

        for (char c : text)
        {
            if (c < 'a' || c > 'z')
                throw std::invalid_argument("Only lowercase a-z allowed");
        }

        int len = static_cast<int>(text.size());
        int value = 0;

        // Step 1: Add all strings with fewer letters
        int shorter_count = 26;
        for (int i = 1; i < len; ++i)
        {
            value += shorter_count;
            shorter_count *= 26;
        }

        // Step 2: Calculate index within same-length strings
        int offset = 0;
        for (char c : text)
        {
            offset = offset * 26 + letterToDecimal(c);
        }

        return value + offset;
    }

    // Convert decimal integer to base-26 string (a=0, b=1, ..., z=25, aa=26, etc.)
    std::string Utils::decimalToBase26(int number)
    {
        if (number < 0)
            throw std::invalid_argument("Negative numbers not allowed");

        std::string result;
        int length = 1;

        // Find how many letters are needed
        int count = 26;
        int remaining = number;
        while (remaining >= count)
        {
            remaining -= count;
            length++;
            count *= 26;
        }

        // Build string from remaining number
        for (int i = 0; i < length; ++i)
        {
            int power = length - i - 1;
            int idx = remaining / static_cast<int>(std::pow(26, power));
            result.push_back(decimalToLetter(idx));
            remaining %= static_cast<int>(std::pow(26, power));
        }

        return result;
    }

    void Utils::sqlite_exec(SQLite::Statement& query)
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

    std::vector<std::string> Utils::split_with_quotes(const std::string& input)
    {
        std::vector<std::string> result;
        std::string current;
        bool in_quotes = false;

        for (size_t i = 0; i < input.size(); ++i)
        {
            char c = input[i];

            if (c == '"')
            {
                in_quotes = !in_quotes;
            }
            else if (c == '\\' && i + 1 < input.size() && input[i + 1] == '"')
            {
                // Escaped quote
                current += '"';
                ++i; // Skip next char
            }
            else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes)
            {
                if (!current.empty())
                {
                    result.push_back(current);
                    current.clear();
                }
            }
            else
            {
                current += c;
            }
        }

        if (!current.empty())
        {
            result.push_back(current);
        }

        return result;
    }

    string Utils::generate_insert_sql(const model::ModelDefinition& definition)
    // const std::string& table_name, const std::vector<const char*>& columns,
    //                                    bool auto_increment)
    {
        string sql = "INSERT INTO " + definition.get_model_name() + " (";
        auto columns = definition.get_columns();
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

    string Utils::generate_update_sql(const model::ModelDefinition& definition)
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

    string Utils::generate_delete_sql(const model::ModelDefinition& definition)
    {
        return "DELETE FROM " + definition.get_model_name() + " WHERE id = ?";
    }

    string Utils::generate_select_one_sql(const std::string& table_name)
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
    string Utils::generate_select_all_sql(const std::string& table_name, const http::QueryParams& query_params,
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

    string Utils::generate_select_count_sql(const std::string& table_name, const http::QueryParams& query_params)
    {
        return generate_select_all_sql(table_name, query_params, true);
    }

    template <class>
    inline constexpr bool always_false = false;

    void Utils::fill_sqlite_query(
        SQLite::Statement& query,
        const entity_fields& values,
        bool auto_increment)
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

        for (size_t i = 0; i < values.size(); ++i)
        {
            if (auto_increment && i == 0)
            {
                continue;
            }
            std::visit([&](auto&& val) -> void
            {
                using T = std::decay_t<decltype(val)>;

                debug << "binding index " << i << " with value \"" << val << "\"" << commit;

                int index = static_cast<int>(i + 1 + (auto_increment ? -1 : 0));
                if constexpr (std::is_same_v<T, std::string>)
                {
                    if (FOREIGN_KEY_NULL == val)
                    {
                        debug << "binding index " << i << " with value NULL" << commit;
                        query.bind(index, nullptr);
                    }
                    else
                    {
                        debug << "binding index " << i << " with value" << commit;
                        query.bind(index, val);
                    }
                }
                else if constexpr (std::is_same_v<T, int64_t>)
                {
                    query.bind(index, val);
                }
                else
                {
                    static_assert(always_false<T>, "Unsupported type in SqlValue");
                }
            }, values[i]);
        }
    }
}
