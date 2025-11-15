///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SUPERADMINLOG_H
#define SUPERADMINLOG_H

#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model SuperAdminLog
#define MODEL SUPER_ADMIN_LOG
#define COLS columns::SuperAdminLogColumns
#include "../columns/SuperAdminLogColumns.hpp"
#include "crow/http_request.h"
#include "mindnet/plugins/core/enums/HttpMethod.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def SUPER_ADMIN_LOG_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("rl")
        .set_title_column(COLS::ENDPOINT)
        .set_group("Core", 600).allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | READONLY).set_description("User ID who performed the request"),
            coldef(COLS::IP_ADDRESS, READONLY).set_description("Client IP address"),
            coldef(COLS::USER_AGENT, READONLY).set_description("Client user agent string"),
            coldef(COLS::ENDPOINT, MANDATORY | READONLY).set_description("Requested API endpoint"),
            coldef(COLS::METHOD, MANDATORY | READONLY).set_enum_definition(
                enums::http_method_to_enum_definition()).set_description("HTTP method"),
            coldef(COLS::ACTION, MANDATORY | READONLY).set_description("Action performed"),
            coldef(COLS::PARAMETERS, READONLY).set_description("Query parameters"),
            coldef(COLS::REQUEST_BODY, READONLY).set_description("Request body"),
            coldef(COLS::DIFF, READONLY).set_description("Differences between change"),
            coldef(COLS::STATUS_CODE, INTEGER | MANDATORY | READONLY).set_description("HTTP response status code"),
            coldef(COLS::ERROR, READONLY).set_description("Error message if request failed"),
            coldef(COLS::SUCCESS, READONLY | BOOL).set_description("Whether the request succeeded")
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        std::string ip_address;
        std::string user_agent;
        std::string endpoint;
        enums::HttpMethod method{enums::HttpMethod::Undefined};
        std::string action;
        std::string parameters;
        std::string request_body;
        std::string diff;
        int status_code{};
        std::string error;
        bool success{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                ip_address == other.ip_address &&
                user_agent == other.user_agent &&
                endpoint == other.endpoint &&
                method == other.method &&
                action == other.action &&
                parameters == other.parameters &&
                request_body == other.request_body &&
                diff == other.diff &&
                status_code == other.status_code &&
                error == other.error &&
                success == other.success;
        }
    };

    const SuperAdminLog super_admin_log_from_crow_request(
        const crow::request& req,
        int user_id,
        int status_code,
        const std::string& action,
        std::string error = "",
        const std::string& diff = "");
}
#undef Model
#undef MODEL
#undef COLS

#endif // SUPERADMINLOG_H
