///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Lightweight wiki inspired by MediaWiki.
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
#ifndef IMPORTANCE_H
#define IMPORTANCE_H

#include <string>

namespace mindnet::Enum
{
    /**
     *
     * @author robertvokac
     */
    enum class Importance
    {
        LOW = 1, MEDIUM = 2, HIGH = 3
    };

    inline std::string importance_to_string(Importance importance)
    {
        switch (importance)
        {
        case Importance::LOW:
            return "Low";
        case Importance::MEDIUM:
            return "Medium";
        case Importance::HIGH:
            return "High";
        default:
            return "Unknown";
        }
    }
    inline std::string importance_to_string(int importance)
    {
        return importance_to_string(static_cast<Importance>(importance));
    }


} // namespace mindnet::Enums
#endif
