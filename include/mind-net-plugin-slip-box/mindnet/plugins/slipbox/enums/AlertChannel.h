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
#ifndef ALERT_CHANNEL_H
#define ALERT_CHANNEL_H

#include <string>
#include "mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::slipbox::enums
{
    /**
     * Defines different channels through which alerts can be delivered
     * @author robertvokac
     */
    enum class AlertChannel
    {
        EMAIL = 0,
        TOAST = 1,
        WEB_NOTIFICATION = 2,
        MOBILE_NOTIFICATION = 3,
        DESKTOP_NOTIFICATION = 4
    };

    inline std::string alert_channel_to_string(AlertChannel channel)
    {
        switch (channel)
        {
        case AlertChannel::EMAIL:
            return "EMAIL";
        case AlertChannel::TOAST:
            return "TOAST";
        case AlertChannel::WEB_NOTIFICATION:
            return "WEB_NOTIFICATION";
        case AlertChannel::MOBILE_NOTIFICATION:
            return "MOBILE_NOTIFICATION";
        case AlertChannel::DESKTOP_NOTIFICATION:
            return "DESKTOP_NOTIFICATION";
        default:
            return "Unknown";
        }
    }

    inline std::string alert_channel_to_string(int channel)
    {
        return alert_channel_to_string(static_cast<AlertChannel>(channel));
    }

    inline model::EnumDefinition alert_channel_to_enum_definition()
    {
        return model::EnumDefinition{
            alert_channel_to_string, 5, 0, 1, 2, 3, 4
        };
    }
} // namespace mindnet::plugins::slipbox::enums
#endif
