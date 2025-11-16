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
#pragma once


#include <string>

#include "mindnet/model/EnumDefinition.hpp"

namespace mindnet::plugins::slipbox::enums
{
    enum class QuestionType
    {
        Single = 0,
        Multi = 1,
        TrueFalse = 2,
        Text = 3
    };

    inline std::string question_type_to_string(const QuestionType type)
    {
        switch (type)
        {
        case QuestionType::Single:
            return "Single";
        case QuestionType::Multi:
            return "Multi";
        case QuestionType::TrueFalse:
            return "TrueFalse";
        case QuestionType::Text:
            return "Text";
        default:
            return "Unknown";
        }
    }

    inline std::string question_type_to_string(int type)
    {
        return question_type_to_string(static_cast<QuestionType>(type));
    }

    inline mindnet::model::EnumDefinition question_type_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            question_type_to_string, 4, 0, 1, 2, 3
        };
    }
} // namespace mindnet::enums

