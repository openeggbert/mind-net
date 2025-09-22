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

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef LOGINSESSIONCOLUMNS_H
#define LOGINSESSIONCOLUMNS_H

#include "../../../model/BaseColumns.h"

namespace mindnet::plugins::core::columns
{
    struct LoginSessionColumns : model::BaseColumns
    {
        LoginSessionColumns() = delete;
        LoginSessionColumns(const LoginSessionColumns&) = delete;
        LoginSessionColumns& operator=(const LoginSessionColumns&) = delete;

        static constexpr const char* MODEL_NAME = "login_session";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* ACCESS_TOKEN_ID = "access_token_id";
        static constexpr const char* REFRESH_TOKEN_ID = "refresh_token_id";
        static constexpr const char* ISSUED_AT = "issued_at";
        static constexpr const char* EXPIRES_AT = "expires_at";
        static constexpr const char* IS_REVOKED = "is_revoked";
        static constexpr const char* REVOKED_AT = "revoked_at";
        static constexpr const char* LAST_USED_AT = "last_used_at";
        static constexpr const char* IP_ADDRESS = "ip_address";
        static constexpr const char* USER_AGENT = "user_agent";
    };
}
#endif // LOGINSESSIONCOLUMNS_H
