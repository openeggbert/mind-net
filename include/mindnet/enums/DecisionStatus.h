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
#ifndef DECISIONSTATUS_H
#define DECISIONSTATUS_H


#include <string>

namespace mindnet::enums
{
    enum class DecisionStatus
    {
        APPROVED = 2,
        REJECTED = 3,
        CANCELLED = 4,
        REQUESTS_FEEDBACK = 5
    };

    inline std::string decision_status_to_string(const DecisionStatus status)
    {
        switch (status)
        {
        case DecisionStatus::APPROVED:
            return "approved";
        case DecisionStatus::REJECTED:
            return "rejected";
        case DecisionStatus::CANCELLED:
            return "cancelled";
        case DecisionStatus::REQUESTS_FEEDBACK:
            return "requests_feedback";
        default:
            return "unknown";
        }
    }

    inline std::string decision_status_to_string(int status)
    {
        return decision_status_to_string(static_cast<DecisionStatus>(status));
    }
} // namespace mindnet::enums

#endif // DECISIONSTATUS_H
