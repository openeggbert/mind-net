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

#include "mindnet/model/EnumDefinition.hpp"

namespace mindnet::plugins::dictionary::enums
{
    enum class SourceType
    {
        Book = 0,
        Article = 1,
        Paper = 2,
        Website = 3,
        Video = 4,
    };

    inline std::string source_type_to_string(const SourceType type)
    {
        switch (type)
        {
        case SourceType::Book:
            return "Book";
        case SourceType::Article:
            return "Article";
        case SourceType::Paper:
            return "Paper";
        case SourceType::Website:
            return "Website";
        case SourceType::Video:
            return "Video";
        default:
            return "Unknown";
        }
    }

    inline std::string source_type_to_string(int type)
    {
        return source_type_to_string(static_cast<SourceType>(type));
    }

    inline mindnet::model::EnumDefinition source_type_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            source_type_to_string, 5, 0, 1, 2, 3, 4
        };
    }
}