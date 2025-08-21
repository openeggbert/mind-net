//
// Created by robertvokac on 7/31/25.
//

#include "mindnet/Utils.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <stdexcept>
#include <regex>

#include "mindnet/Global.h"
#include "SQLiteCpp/Database.h"

namespace mindnet
{
    namespace Entity
    {
        struct BaseModel;
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

    void Utils::log(const std::string& msg)
    {
        std::cout << "[INFO] " << msg << std::endl;
    }

    void Utils::log(const char* message)
    {
        log(std::string(message));
    } // MiniWiki
    void Utils::trace(const char* message)
    {
        //std::cout << "[TRACE] " << message << std::endl;
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
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
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

    str Utils::generate_insert_sql(const models::ModelDefinition& definition)
    // const std::string& table_name, const std::vector<const char*>& columns,
    //                                    bool auto_increment)
    {
        str sql = "INSERT INTO " + definition.model_name + " (";
        auto columns = definition.columns;
        for (int i = 0; i < columns.size(); ++i)
        {
            auto& column = columns[i].column_name;
            if (definition.auto_increment && std::string(column) == PRIMARY_KEY_COLUMN_NAME)
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
            auto column = columns[i].column_name;
            if (definition.auto_increment && std::string(column) == PRIMARY_KEY_COLUMN_NAME)
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

    str Utils::generate_update_sql(const models::ModelDefinition& definition)
    {
        std::string sql = "UPDATE " + definition.model_name + " SET ";
        for (int i = 0; i < definition.columns.size(); ++i)
        {
            auto column = definition.columns[i].column_name;
            if (definition.auto_increment && std::string(column) == PRIMARY_KEY_COLUMN_NAME)
            {
                continue;
            }
            sql += column + "=?";
            if (i < definition.columns.size() - 1)
            {
                sql += ", ";
            }
        }

        sql += " WHERE ID = ?";
        return sql;
    }

    str Utils::generate_delete_sql(const models::ModelDefinition& definition)
    {
        return "DELETE FROM " + definition.model_name + " WHERE id = ?";

    }

    str Utils::generate_select_one_sql(const std::string& table_name)
    {
        return "SELECT * FROM " + table_name + " WHERE id = ?";
    }


    template <class>
    inline constexpr bool always_false = false;

    void Utils::fill_sqlite_query(SQLite::Statement& query, const entity_fields& values, bool auto_increment)
    {
        std::size_t values_size = values.size();
        if (auto_increment) { values_size--; }
        if (values_size > static_cast<size_t>(query.getBindParameterCount()))
        {
            err << "values().size " << values_size << " > query.getBindParameterCount() " << static_cast<size_t>(query.
                getBindParameterCount()) << std::endl;
            throw std::out_of_range("More values provided than query parameters");
        }

        for (size_t i = 0; i < values.size(); ++i)
        {
            if (auto_increment && i == 0)
            {
                continue;
            }
            std::visit([&](auto&& val) -> void
            {
                err << "binding index " << i << " with value \"" << val << "\"" << std::endl;

                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, std::string>)
                {
                    query.bind(static_cast<int>(i + 1 + (auto_increment ? -1 : 0)), val);
                }
                else if constexpr (std::is_same_v<T, int64_t>)
                {
                    query.bind(static_cast<int>(i + 1 + (auto_increment ? -1 : 0)), val);
                }
                else
                {
                    static_assert(always_false<T>, "Unsupported type in SqlValue");
                }
            }, values[i]);
        }
    }
}
