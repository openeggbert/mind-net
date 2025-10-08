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
#ifndef REPETITION_SCOPE_H
#define REPETITION_SCOPE_H

#include <string>

#include "mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::repetition::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class RepetitionScope
    {
        Manual = 0,
        DueOnly = 1,
        NewOnly = 2,
        DueAndNew = 3,
    };

    inline std::string repetition_scope_to_string(RepetitionScope scope)
    {
        switch (scope)
        {
        case RepetitionScope::Manual:
            return "Manual";
        case RepetitionScope::DueOnly:
            return "DueOnly";
        case RepetitionScope::NewOnly:
            return "NewOnly";
        case RepetitionScope::DueAndNew:
            return "DueAndNew";
        default:
            return "Unknown";
        }
    }

    inline std::string repetition_scope_to_string(int scope)
    {
        return repetition_scope_to_string(static_cast<RepetitionScope>(scope));
    }

    inline model::EnumDefinition repetition_scope_to_enum_definition()
    {
        return model::EnumDefinition{
            repetition_scope_to_string, 4, 0, 1, 2, 3
        };
    }
} // namespace mindnet::plugins::repetition::enums
#endif
