//
// Created by robertvokac on 8/16/25.
//
#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <string>

#include "PrimitiveColumnType.hpp"

namespace mindnet::model
{
    enum class ColumnType
    {
        Text,
        TextArea,
        Integer,
        Bool,
        DateTime,
        Real,
        Blob,
    };

    inline std::string column_type_to_string(ColumnType column_type)
    {
        switch (column_type)
        {
        case ColumnType::Text:
            return "Text";
        case ColumnType::TextArea:
            return "TextArea";
        case ColumnType::Integer:
            return "Integer";
        case ColumnType::Real:
            return "Real";
        case ColumnType::Blob:
            return "Blob";
        case ColumnType::Bool:
            return "Bool";
        case ColumnType::DateTime:
            return "DateTime";
        default:
            return "Unknown`";
        }
    }

    inline std::string column_type_to_string(int column_type)
    {
        return column_type_to_string(static_cast<ColumnType>(column_type));
    }

    inline PrimitiveColumnType column_type_to_primitive_column_type(const ColumnType& column_type)
    {
        switch (column_type)
        {
        case ColumnType::Text:
        case ColumnType::TextArea:
            return PrimitiveColumnType::Text;
        case ColumnType::Integer:
        case ColumnType::Bool:
        case ColumnType::DateTime:
            return PrimitiveColumnType::Number;

        case ColumnType::Real:
        case ColumnType::Blob:
        default:
            return PrimitiveColumnType::Unknown;
        }
    }
}

#endif // COLUMNTYPE_H
