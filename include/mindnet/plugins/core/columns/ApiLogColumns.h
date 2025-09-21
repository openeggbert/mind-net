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
#ifndef APILOGCOLUMNS_H
#define APILOGCOLUMNS_H


#include "../../../model/BaseColumns.h"

namespace mindnet::plugins::core::columns
{
    struct ApiLogColumns : model::BaseColumns
    {
        ApiLogColumns() = delete;
        ApiLogColumns(const ApiLogColumns&) = delete;
        ApiLogColumns& operator=(const ApiLogColumns&) = delete;

        static constexpr const char* MODEL_NAME    = "api_log";

        // who
        static constexpr const char* USER_ID       = "user_id";
        static constexpr const char* IP_ADDRESS    = "ip_address";

        // what
        static constexpr const char* ENDPOINT      = "endpoint";
        static constexpr const char* METHOD        = "method";
        static constexpr const char* ENTITY_NAME   = "entity_name";
        static constexpr const char* ENTITY_ID     = "entity_id";
        static constexpr const char* PARAMETERS    = "parameters";
        static constexpr const char* REQUEST_BODY  = "request_body";

        // result
        static constexpr const char* STATUS_CODE   = "status_code";
        static constexpr const char* ERROR   = "error";
    };
}

#endif // APILOGCOLUMNS_H
