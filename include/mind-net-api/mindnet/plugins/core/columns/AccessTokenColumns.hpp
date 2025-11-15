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
#ifndef ACCESSTOKENCOLUMNS_H
#define ACCESSTOKENCOLUMNS_H

#include "mindnet/model/BaseColumns.hpp"

namespace mindnet::plugins::core::columns
{
    struct AccessTokenColumns : model::BaseColumns
    {
        AccessTokenColumns() = delete;

        AccessTokenColumns(const AccessTokenColumns&) = delete;
        AccessTokenColumns& operator=(const AccessTokenColumns&) = delete;

        static constexpr const char* MODEL_NAME = "access_token";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* TOKEN_HASH = "token_hash";
        static constexpr const char* TOKEN_PURPOSE = "token_purpose";
        static constexpr const char* IS_CREATED_BY_USER = "is_created_by_user";
        static constexpr const char* NAME = "name";
        static constexpr const char* DESCRIPTION = "description";
        static constexpr const char* ISSUED_AT = "issued_at";
        static constexpr const char* EXPIRES_AT = "expires_at";
        static constexpr const char* IS_REVOKED = "is_revoked";
        static constexpr const char* REVOKED_AT = "revoked_at";
        static constexpr const char* LAST_USED_AT = "last_used_at";
        static constexpr const char* IP_ADDRESS = "ip_address";
        static constexpr const char* USER_AGENT = "user_agent";
        static constexpr const char* ALLOWED_OPERATIONS = "allowed_operations";
    };
}
#endif // ACCESSTOKENCOLUMNS_H
