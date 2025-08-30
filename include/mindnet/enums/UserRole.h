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
#ifndef USERROLE_H
#define USERROLE_H


#include <string>

#include "mindnet/models/misc/EnumDefinition.h"

namespace mindnet::enums
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
     *
     * During the first application launch, Mind Net creates two users with usernames: admin and editor, both the users have the empty passwords.
     */
    enum class UserRole
    {
        GUEST = 0,
        READER = 1,
        EDITOR = 2,
        REVIEWER = 3,
        ADMIN = 4
    };

    inline std::string user_role_to_string(const UserRole role)
    {
        switch (role)
        {
        case UserRole::GUEST:
            return "Guest";
        case UserRole::READER:
            return "Reader";
        case UserRole::EDITOR:
            return "Editor";
        case UserRole::REVIEWER:
            return "Reviewer";
        case UserRole::ADMIN:
            return "Admin";
        default:
            return "Unknown";
        }
    }

    inline std::string user_role_to_string(int role)
    {
        return user_role_to_string(static_cast<UserRole>(role));
    }

    inline models::misc::EnumDefinition user_role_to_enum_definition()
    {
        return models::misc::EnumDefinition{
            user_role_to_string, 5, 0, 1, 2, 3, 4
        };
    }
} // namespace mindnet::enums

#endif // USERROLE_H
