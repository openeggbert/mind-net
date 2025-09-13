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
#ifndef USERSTATUS_H
#define USERSTATUS_H

#include <string>

#include "../../../model/EnumDefinition.h"

namespace mindnet::plugins::core::enums
{
    /**
     * @enum UserStatus
     * @brief Enumerates different statuses a user can have within the system.
     *
     * This enumeration defines various user statuses for managing user accounts:
     * - PENDING: Account created but not yet activated
     * - ACTIVE: Normal active account 
     * - DEACTIVATED: Account manually deactivated
     * - BANNED: Account banned for violations
     * - SUSPENDED: Account temporarily suspended
     * - DELETED: Account marked as deleted
     */
    enum class UserStatus
    {
        PENDING = 0,
        ACTIVE = 1,
        DEACTIVATED = 2,
        BANNED = 3,
        SUSPENDED = 4,
        DELETED = 5
    };

    inline std::string user_status_to_string(const UserStatus status)
    {
        switch (status)
        {
        case UserStatus::PENDING:
            return "pending";
        case UserStatus::ACTIVE:
            return "active";
        case UserStatus::DEACTIVATED:
            return "deactivated";
        case UserStatus::BANNED:
            return "banned";
        case UserStatus::SUSPENDED:
            return "suspended";
        case UserStatus::DELETED:
            return "deleted";
        default:
            return "unknown";
        }
    }

    inline std::string user_status_to_string(int status)
    {
        return user_status_to_string(static_cast<UserStatus>(status));
    }

    inline mindnet::model::EnumDefinition user_status_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            user_status_to_string, 6, 0, 1, 2, 3, 4, 5
        };
    }
} // namespace mindnet::enums
#endif // USERSTATUS_H
