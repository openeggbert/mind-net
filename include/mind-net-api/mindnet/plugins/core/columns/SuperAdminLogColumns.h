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
#ifndef SUPERADMINLOGCOLUMNS_H
#define SUPERADMINLOGCOLUMNS_H

#include "mindnet/model/BaseColumns.h"

namespace mindnet::plugins::core::columns
{
    struct SuperAdminLogColumns : model::BaseColumns
    {
        SuperAdminLogColumns() = delete;
        SuperAdminLogColumns(const SuperAdminLogColumns&) = delete;
        SuperAdminLogColumns& operator=(const SuperAdminLogColumns&) = delete;

        static constexpr const char* MODEL_NAME = "super_admin_log";

        // who
        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* IP_ADDRESS = "ip_address";
        static constexpr const char* USER_AGENT = "user_agent";

        // what was called
        static constexpr const char* ENDPOINT = "endpoint";
        static constexpr const char* METHOD = "method";
        static constexpr const char* ACTION = "action";

        // context
        static constexpr const char* PARAMETERS = "parameters";
        static constexpr const char* REQUEST_BODY = "request_body";
        static constexpr const char* DIFF = "diff";

        // result
        static constexpr const char* STATUS_CODE = "status_code";
        static constexpr const char* ERROR = "error";
        static constexpr const char* SUCCESS = "success";
    };
}

#endif // SUPERADMINLOGCOLUMNS_H
