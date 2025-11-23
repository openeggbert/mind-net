/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once


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

