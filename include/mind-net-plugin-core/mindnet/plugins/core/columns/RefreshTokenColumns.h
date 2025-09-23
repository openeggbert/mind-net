/*
 * mind-net : A generic, high-performance REST backend framework written in modern C++.
 * Copyright (C) 2025 the original author or authors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <https://www.gnu.org/licenses/> or write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef REFRESHTOKENCOLUMNS_H
#define REFRESHTOKENCOLUMNS_H

#include "../../../../../mind-net-model/mindnet/model/BaseColumns.h"

namespace mindnet::plugins::core::columns
{
    struct RefreshTokenColumns : model::BaseColumns
    {
        RefreshTokenColumns() = delete;
        RefreshTokenColumns(const RefreshTokenColumns&) = delete;
        RefreshTokenColumns& operator=(const RefreshTokenColumns&) = delete;

        static constexpr const char* MODEL_NAME      = "refresh_token";

        static constexpr const char* USER_ID         = "user_id";
        static constexpr const char* TOKEN_HASH      = "token_hash";
        static constexpr const char* ISSUED_AT       = "issued_at";
        static constexpr const char* EXPIRES_AT      = "expires_at";
        static constexpr const char* IS_REVOKED      = "is_revoked";
        static constexpr const char* REVOKED_AT      = "revoked_at";
        static constexpr const char* REPLACED_BY_ID  = "replaced_by_id";
        static constexpr const char* ROTATED_FROM_ID = "rotated_from_id";
        static constexpr const char* LAST_USED_AT    = "last_used_at";
        static constexpr const char* IP_ADDRESS      = "ip_address";
        static constexpr const char* USER_AGENT      = "user_agent";
    };
}

#endif // REFRESHTOKENCOLUMNS_H
