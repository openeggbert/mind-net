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
#ifndef SINGLERIGHT_H
#define SINGLERIGHT_H


#include <string>

#include "../../../model/EnumDefinition.h"

namespace mindnet::plugins::core::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class SingleRight
    {
        NONE = 0,
        READ = 1,
        WRITE = 2,
        DELETE = 3
    };

    inline std::string single_right_to_string(SingleRight single_right)
    {
        switch (single_right)
        {
        case SingleRight::NONE:
            return "None";
        case SingleRight::DELETE:
            return "Delete";
        case SingleRight::WRITE:
            return "Write";
        case SingleRight::READ:
            return "Read";
        default:
            return "Unknown";
        }
    }

    inline std::string single_right_to_string(int single_right)
    {
        return single_right_to_string(static_cast<SingleRight>(single_right));
    }

    inline mindnet::model::EnumDefinition single_right_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            single_right_to_string, 4, 0, 1, 2, 3
        };
    }

} // namespace mindnet::enums

#endif // SINGLERIGHT_H
