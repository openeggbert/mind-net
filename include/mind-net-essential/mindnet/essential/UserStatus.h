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
#ifndef USERSTATUS_H
#define USERSTATUS_H

#include <string>

namespace mindnet::essential
{
    /**
     * @enum UserStatus
     * @brief Enumerates different statuses a user can have within the system.
     *
     * This enumeration defines various user statuses for managing user accounts:
     * - Pending: Account created but not yet activated
     * - Active: Normal active account
     * - Deactivated: Account manually deactivated
     * - Banned: Account banned for violations
     * - Suspended: Account temporarily suspended
     * - Deleted: Account marked as deleted
     */
    enum class UserStatus
    {
        Pending = 0,
        Active = 1,
        Deactivated = 2,
        Banned = 3,
        Suspended = 4,
        Deleted = 5
    };

    inline std::string user_status_to_string(const UserStatus status)
    {
        switch (status)
        {
        case UserStatus::Pending:
            return "Pending";
        case UserStatus::Active:
            return "Active";
        case UserStatus::Deactivated:
            return "Deactivated";
        case UserStatus::Banned:
            return "Banned";
        case UserStatus::Suspended:
            return "Suspended";
        case UserStatus::Deleted:
            return "Deleted";
        default:
            return "Unknown";
        }
    }

    inline std::string user_status_to_string(int status)
    {
        return user_status_to_string(static_cast<UserStatus>(status));
    }

}
#endif // USERSTATUS_H
