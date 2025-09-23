///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see 
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SOURCETYPE_H
#define SOURCETYPE_H


#include <string>

#include "../../../../../mind-net-model/mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::slipbox::enums
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
            source_type_to_string, 4, 0, 1, 2, 3
        };
    }
} // namespace mindnet::enums

#endif // SOURCETYPE_H
