///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef QUESTIONTYPE_H
#define QUESTIONTYPE_H

#include <string>

#include "mindnet/models/misc/EnumDefinition.h"

namespace mindnet::enums
{
    enum class QuestionType
    {
        SINGLE = 0,
        MULTI = 1,
        TRUEFALSE = 2,
        TEXT = 3
    };

    inline std::string question_type_to_string(const QuestionType type)
    {
        switch (type)
        {
        case QuestionType::SINGLE:
            return "single";
        case QuestionType::MULTI:
            return "multi";
        case QuestionType::TRUEFALSE:
            return "truefalse";
        case QuestionType::TEXT:
            return "text";
        default:
            return "unknown";
        }
    }

    inline std::string question_type_to_string(int type)
    {
        return question_type_to_string(static_cast<QuestionType>(type));
    }

    inline models::misc::EnumDefinition question_type_to_enum_definition()
    {
        return models::misc::EnumDefinition{
            question_type_to_string, 4, 0, 1, 2, 3
        };
    }
} // namespace mindnet::enums

#endif // QUESTIONTYPE_H
