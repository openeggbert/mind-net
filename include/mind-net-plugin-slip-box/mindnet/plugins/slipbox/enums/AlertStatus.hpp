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
#ifndef ALERT_STATUS_H
#define ALERT_STATUS_H

#include <string>
#include "mindnet/model/EnumDefinition.hpp"

namespace mindnet::plugins::slipbox::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class AlertStatus
    {
        PENDING = 0,
        TRIGGERED = 1,
        SNOOZED = 2,
        DISMISSED = 3,
        FAILED = 4
    };

    inline std::string alert_status_to_string(AlertStatus status)
    {
        switch (status)
        {
        case AlertStatus::PENDING:
            return "PENDING";
        case AlertStatus::TRIGGERED:
            return "TRIGGERED";
        case AlertStatus::SNOOZED:
            return "SNOOZED";
        case AlertStatus::DISMISSED:
            return "DISMISSED";
        case AlertStatus::FAILED:
            return "FAILED";
        default:
            return "Unknown";
        }
    }

    inline std::string alert_status_to_string(int status)
    {
        return alert_status_to_string(static_cast<AlertStatus>(status));
    }

    inline model::EnumDefinition alert_status_to_enum_definition()
    {
        return model::EnumDefinition{
            alert_status_to_string, 5, 0, 1, 2, 3, 4
        };
    }
} // namespace mindnet::plugins::slipbox::enums
#endif
