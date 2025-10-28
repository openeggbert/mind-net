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
#ifndef USERSTATUS_H_PLUGIN_CORE
#define USERSTATUS_H_PLUGIN_CORE

#include <string>

#include "mindnet/essential/UserStatus.h"
#include "mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::core::enums
{
    inline mindnet::model::EnumDefinition user_status_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            essential::user_status_to_string, 6, 0, 1, 2, 3, 4, 5
        };
    }
} // namespace mindnet::enums
#endif // USERSTATUS_H_PLUGIN_CORE
