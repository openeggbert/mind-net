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
#ifndef VISIBILITY_H
#define VISIBILITY_H


#include <string>

namespace mindnet::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class Visibility
    {
        PUBLIC = 0, PRIVATE = 1, DRAFT = 2, ARCHIVED = 3
    };

    inline std::string visibility_to_string(Visibility visibility)
    {
        switch (visibility)
        {
        case Visibility::PUBLIC:
            return "Public";
        case Visibility::PRIVATE:
            return "Private";
        case Visibility::DRAFT:
            return "Draft";
        case Visibility::ARCHIVED:
            return "Archived";
        default:
            return "Unknown";
        }
    }

    inline std::string visibility_to_string(int visibility)
    {
        return visibility_to_string(static_cast<Visibility>(visibility));
    }
} // namespace mindnet::enums
#endif // VISIBILITY_H
