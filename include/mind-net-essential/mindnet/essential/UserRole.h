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
#ifndef USERROLE_H
#define USERROLE_H

#include <stdexcept>
#include <string>
#include <vector>

namespace mindnet::essential
{
    /**
     * @enum UserRole
     * @brief Enumerates different roles a user can have within the system.
     *
     * This enumeration defines various user roles with associated permissions
     * and responsibilities. The roles are ordered hierarchically based on their
     * authority level:
     *
     * - GUEST: Any user, which is not logged in. Guest cannot modify content or send suggestions.
     * - READER: Can read. Can create change requests (that must be approved to be applied)
     * - EDITOR: Can modify without approval of someone else.
     * - REVIEWER: The same as editor, but additionally can approve suggested changes by readers.
     * - ADMIN: Can do anything.
     * - SUPER_ADMIN: Has all ADMIN privileges plus system-level controls including server management (restart/shutdown) and runtime configuration modifications through the web interface.
     * - SYSTEM: Can do anything and even more than ADMIN.
     * During the first application launch, Mind Net creates two users with usernames: admin and editor, both the users have the empty passwords.
     */
    enum class UserRole
    {
        Guest = 0,
        Reader = 1,
        Editor = 2,
        Reviewer = 3,
        Admin = 4,
        SuperAdmin = 5,
        System = 100
    };

    inline std::string user_role_to_string(const UserRole role)
    {
        switch (role)
        {
        case UserRole::Guest:
            return "Guest";
        case UserRole::Reader:
            return "Reader";
        case UserRole::Editor:
            return "Editor";
        case UserRole::Reviewer:
            return "Reviewer";
        case UserRole::Admin:
            return "Admin";
        case UserRole::SuperAdmin:
            return "SuperAdmin";
        case UserRole::System:
            return "System";
        default:
            return "Unknown";
        }
    }

    inline std::vector<UserRole> user_role_to_values()
    {
        static std::vector<UserRole> values = {
            UserRole::Guest,
            UserRole::Reader,
            UserRole::Editor,
            UserRole::Reviewer,
            UserRole::Admin,
            UserRole::SuperAdmin,
            UserRole::System
        };
        return values;
    }

    inline std::string user_role_to_string(int role)
    {
        return user_role_to_string(static_cast<UserRole>(role));
    }

    inline UserRole string_to_user_role(const std::string& role_str)
    {
        if (role_str == "Guest") return UserRole::Guest;
        if (role_str == "Reader") return UserRole::Reader;
        if (role_str == "Editor") return UserRole::Editor;
        if (role_str == "Reviewer") return UserRole::Reviewer;
        if (role_str == "Admin") return UserRole::Admin;
        if (role_str == "SuperAdmin") return UserRole::SuperAdmin;
        if (role_str == "System") return UserRole::System;
        throw std::runtime_error("Invalid user role: " + role_str);
    }

}

#endif // USERROLE_H