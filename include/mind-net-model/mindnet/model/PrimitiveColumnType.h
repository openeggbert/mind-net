//
// Created by robertvokac on 8/16/25.
//
#ifndef PRIMITIVECOLUMNTYPE_H
#define PRIMITIVECOLUMNTYPE_H

#include <string>

namespace mindnet::model
{
    enum class PrimitiveColumnType
    {
        Unknown = 0,
        Text,
        Number
    };

    inline std::string primitive_column_type_to_string(PrimitiveColumnType primitive_column_type)
    {
        switch (primitive_column_type)
        {
        case PrimitiveColumnType::Text:
            return "Text";
        case PrimitiveColumnType::Number:
            return "Number";
        default:
            return "Unknown";
        }
    }

    inline std::string primitive_column_type_to_string(int primitive_column_type)
    {
        return primitive_column_type_to_string(static_cast<PrimitiveColumnType>(primitive_column_type));
    }

}

#endif // PRIMITIVECOLUMNTYPE_H
