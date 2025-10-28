//
// Created by robertvokac on 9/17/25.
//

#ifndef MIND_NET_DATABASETYPE_H
#define MIND_NET_DATABASETYPE_H
#include <string>

namespace mindnet::essential
{
    enum class DatabaseType
    {
        Unknown = 0,
        SQLite = 1,
        PostgreSQL = 2,
        MariaDB = 3,
        MySQL = 4,
    };

    inline bool is_database_type_supported(DatabaseType database_type)
    {
        switch (database_type)
        {
        case DatabaseType::SQLite: return true;
        default: return false;
        }
    }

    inline std::string database_type_to_string(DatabaseType database_type)
    {
        switch (database_type)
        {
        case DatabaseType::Unknown:
            return "Unknown";
        case DatabaseType::SQLite:
            return "SQLite";
        case DatabaseType::PostgreSQL:
            return "PostgreSQL";
        case DatabaseType::MariaDB:
            return "MariaDB";
        case DatabaseType::MySQL:
            return "MySQL";
        default:
            return "Unknown";
        }
    }

    inline std::string database_type_to_string(int database_type)
    {
        return database_type_to_string(static_cast<DatabaseType>(database_type));
    }

    inline DatabaseType string_to_database_type(const std::string& type_str)
    {
        if (type_str == "Unknown") return DatabaseType::Unknown;
        if (type_str == "SQLite") return DatabaseType::SQLite;
        if (type_str == "PostgreSQL") return DatabaseType::PostgreSQL;
        if (type_str == "MariaDB") return DatabaseType::MariaDB;
        if (type_str == "MySQL") return DatabaseType::MySQL;
        throw std::runtime_error("Invalid database type: " + type_str);
    }

    inline std::vector<DatabaseType> database_type_to_values()
    {
        static std::vector values = {
            DatabaseType::Unknown,
            DatabaseType::SQLite,
            DatabaseType::PostgreSQL,
            DatabaseType::MariaDB,
            DatabaseType::MySQL
        };
        return values;
    }

    // inline mindnet::model::EnumDefinition database_type_to_enum_definition()
    // {
    //     return mindnet::model::EnumDefinition{
    //         database_type_to_string, 5, 0, 1, 2, 3, 4
    //     };
    // }
}
#endif //MIND_NET_DATABASETYPE_H
