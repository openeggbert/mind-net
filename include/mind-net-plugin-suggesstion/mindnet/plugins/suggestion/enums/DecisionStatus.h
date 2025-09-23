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
#ifndef DECISIONSTATUS_H
#define DECISIONSTATUS_H

#include <string>

#include "../../../../../mind-net-model/mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::suggestion::enums
{
    enum class DecisionStatus
    {
        Approved = 2,
        Rejected = 3,
        Cancelled = 4,
        RequestsFeedback = 5
    };

    inline std::string decision_status_to_string(const DecisionStatus status)
    {
        switch (status)
        {
        case DecisionStatus::Approved:
            return "Approved";
        case DecisionStatus::Rejected:
            return "Rejected";
        case DecisionStatus::Cancelled:
            return "Cancelled";
        case DecisionStatus::RequestsFeedback:
            return "RequestsFeedback";
        default:
            return "Unknown";
        }
    }

    inline std::string decision_status_to_string(int status)
    {
        return decision_status_to_string(static_cast<DecisionStatus>(status));
    }

    inline mindnet::model::EnumDefinition decision_status_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            decision_status_to_string, 4, 2, 3, 4, 5
        };
    }
} // namespace mindnet::enums

#endif // DECISIONSTATUS_H
