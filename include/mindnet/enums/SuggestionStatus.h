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
#ifndef SUGGESTIONSTATUS_H
#define SUGGESTIONSTATUS_H

#include <string>

namespace mindnet::enums
{
    enum class SuggestionStatus
    {
        PENDING = 0,
        UNDER_REVIEW = 1,
        APPROVED = 2,
        REJECTED = 3,
        CANCELLED = 4,
        REQUESTS_FEEDBACK = 5
    };

    inline std::string suggestion_status_to_string(const SuggestionStatus status)
    {
        switch (status)
        {
        case SuggestionStatus::PENDING:
            return "pending";
        case SuggestionStatus::UNDER_REVIEW:
            return "under_review";
        case SuggestionStatus::APPROVED:
            return "approved";
        case SuggestionStatus::REJECTED:
            return "rejected";
        case SuggestionStatus::CANCELLED:
            return "cancelled";
        case SuggestionStatus::REQUESTS_FEEDBACK:
            return "requests_feedback";
        default:
            return "unknown";
        }
    }

    inline std::string suggestion_status_to_string(int status)
    {
        return suggestion_status_to_string(static_cast<SuggestionStatus>(status));
    }
} // namespace mindnet::enums

#endif // SUGGESTIONSTATUS_H
