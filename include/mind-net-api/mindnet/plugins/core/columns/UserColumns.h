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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef USERCOLUMNS_H
#define USERCOLUMNS_H

#include "mindnet/model/BaseColumns.h"

namespace mindnet::plugins::core::columns
{
    struct UserColumns : model::BaseColumns
    {
        UserColumns() = delete;

        UserColumns(const UserColumns&) = delete;
        UserColumns& operator=(const UserColumns&) = delete;

        static constexpr const char* MODEL_NAME = "user";

        // Authentication fields
        static constexpr const char* USERNAME = "username";
        static constexpr const char* PASSWORD_HASH = "password_hash";
        static constexpr const char* EMAIL = "email";

        // Profile fields
        static constexpr const char* DISPLAY_NAME = "display_name";
        static constexpr const char* PROFILE_TEXT = "profile_text";

        // Access control fields
        static constexpr const char* ROLE = "role";
        static constexpr const char* STATUS = "status";

        // Tracking field
        static constexpr const char* LAST_LOGIN = "last_login";
    };
}
#endif // USERCOLUMNS_H
