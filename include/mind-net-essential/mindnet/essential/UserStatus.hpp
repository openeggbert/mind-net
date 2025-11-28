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
