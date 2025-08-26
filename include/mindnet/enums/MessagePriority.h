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
#ifndef MESSAGE_PRIORITY_H
#define MESSAGE_PRIORITY_H

#include <string>

#include "mindnet/models/misc/EnumDefinition.h"

namespace mindnet::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class MessagePriority
    {
        UNDEFINED = 0, LOW = 1, NORMAL = 2, HIGH = 3
    };

    inline std::string message_priority_to_string(MessagePriority priority)
    {
        switch (priority)
        {
        case MessagePriority::UNDEFINED:
            return "Undefined";
        case MessagePriority::LOW:
            return "Low";
        case MessagePriority::NORMAL:
            return "Normal";
        case MessagePriority::HIGH:
            return "High";
        default:
            return "Unknown";
        }
    }

    inline std::string message_priority_to_string(int priority)
    {
        return message_priority_to_string(static_cast<MessagePriority>(priority));
    }

    inline models::misc::EnumDefinition message_priority_to_enum_definition()
    {
        return models::misc::EnumDefinition{
            message_priority_to_string, 4, 0, 1, 2, 3
        };
    }
} // namespace mindnet::enums
#endif
