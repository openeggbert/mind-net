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
#ifndef NODELINKTYPE_H
#define NODELINKTYPE_H

#include <string>

namespace mindnet::enums
{
    enum class NodeLinkType
    {
        MANUAL = 0,
        PARSED = 1
    };

    inline std::string node_link_type_to_string(const NodeLinkType type)
    {
        switch (type)
        {
        case NodeLinkType::MANUAL:
            return "manual";
        case NodeLinkType::PARSED:
            return "parsed";
        default:
            return "unknown";
        }
    }

    inline std::string node_link_type_to_string(int type)
    {
        return node_link_type_to_string(static_cast<NodeLinkType>(type));
    }
} // namespace mindnet::enums

#endif // NODELINKTYPE_H
