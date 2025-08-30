//
// Created by robertvokac on 8/16/25.
//
#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <string>

namespace mindnet::enums
{
    enum class ColumnType
    {
        TEXT,
        TEXTAREA,
        INTEGER,
        BOOL,
        DATETIME,
        REAL,
        BLOB,
    };

    inline std::string column_type_to_string(ColumnType column_type)
    {
        switch (column_type)
        {
        case ColumnType::TEXT:
            return "TEXT";
        case ColumnType::TEXTAREA:
            return "TEXTAREA";
        case ColumnType::INTEGER:
            return "INTEGER";
        case ColumnType::REAL:
            return "REAL";
        case ColumnType::BLOB:
            return "BLOB";
        case ColumnType::BOOL:
            return "BOOL";
        case ColumnType::DATETIME:
            return "DATETIME";
        default:
            return "UNKNOWN";
        }
    }

    inline std::string column_type_to_string(int column_type)
    {
        return column_type_to_string(static_cast<ColumnType>(column_type));
    }
}

#endif // COLUMNTYPE_H
