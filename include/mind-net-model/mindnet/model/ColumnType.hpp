/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once



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

