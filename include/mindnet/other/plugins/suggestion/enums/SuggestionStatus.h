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
#ifndef SUGGESTIONSTATUS_H
#define SUGGESTIONSTATUS_H

#include <string>

#include "../../../model/EnumDefinition.h"

namespace mindnet::plugins::suggestion::enums
{
    enum class SuggestionStatus
    {
        Draft = 0,
        Pending = 1,
        UnderReview = 2,
        Approved = 3,
        Rejected = 4,
        Cancelled = 5,
        RequestsFeedback = 6
    };

    inline std::string suggestion_status_to_string(const SuggestionStatus status)
    {
        switch (status)
        {
        case SuggestionStatus::Draft:
            return "Draft";
        case SuggestionStatus::Pending:
            return "Pending";
        case SuggestionStatus::UnderReview:
            return "UnderReview";
        case SuggestionStatus::Approved:
            return "Approved";
        case SuggestionStatus::Rejected:
            return "Rejected";
        case SuggestionStatus::Cancelled:
            return "Cancelled";
        case SuggestionStatus::RequestsFeedback:
            return "RequestsFeedback";
        default:
            return "Unknown";
        }
    }

    inline std::string suggestion_status_to_string(int status)
    {
        return suggestion_status_to_string(static_cast<SuggestionStatus>(status));
    }

    inline mindnet::model::EnumDefinition suggestion_status_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            suggestion_status_to_string, 7, 0, 1, 2, 3, 4, 5, 6
        };
    }
} // namespace mindnet::enums

#endif // SUGGESTIONSTATUS_H
