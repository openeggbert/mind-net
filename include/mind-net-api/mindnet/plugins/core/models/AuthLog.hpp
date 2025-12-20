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
#include "mindnet/model/BaseModel.hpp"
#include "mindnet/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model AuthLog
#define MODEL AUTH_LOG
#define COLS columns::AuthLogColumns
#include "../columns/AuthLogColumns.hpp"
#include "crow/http_request.h"
#include "mindnet/plugins/core/enums/HttpMethod.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def AUTH_LOG_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_title_column(COLS::ENDPOINT)
        .set_group("Core", 600).allow_reader_write()
        .set_columns({
            //
            coldef(COLS::USER_ID, FOREIGN_KEY | READONLY).set_description("User ID who performed the request"),
            coldef(COLS::IP_ADDRESS, READONLY).set_description("Client IP address"),
            coldef(COLS::USER_AGENT, READONLY).set_description("Client user agent string"),
            coldef(COLS::ENDPOINT, MANDATORY | READONLY).set_description("Requested API endpoint"),
            coldef(COLS::METHOD, MANDATORY | READONLY).set_enum_definition(
                enums::http_method_to_enum_definition()).set_description("HTTP method"),
            coldef(COLS::ACTION, READONLY).set_description("Action performed"),
            coldef(COLS::ENTITY_NAME, MANDATORY | READONLY).set_description("Target model name"),
            coldef(COLS::ENTITY_ID, READONLY).set_description("Target entity ID"),
            coldef(COLS::PARAMETERS, READONLY).set_description("Query parameters"),
            coldef(COLS::REQUEST_BODY, READONLY).set_description("Request body"),
            coldef(COLS::STATUS_CODE, INTEGER | MANDATORY | READONLY).set_description("HTTP response status code"),
            coldef(COLS::ERROR, READONLY).set_description("Error message if request failed"),
            coldef(COLS::SUCCESS, READONLY | BOOL).set_description("Whether the request succeeded")
            //
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        std::string ip_address;
        std::string user_agent;
        std::string endpoint;
        enums::HttpMethod method{enums::HttpMethod::Undefined};
        std::string action;
        std::string entity_name;
        identification entity_id{};
        std::string parameters;
        std::string request_body;
        int status_code{};
        std::string error;
        bool success{false};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::ip_address,
            &Model::user_agent,
            &Model::endpoint,
            &Model::method,
            &Model::action,
            &Model::entity_name,
            &Model::entity_id,
            &Model::parameters,
            &Model::request_body,
            &Model::status_code,
            &Model::error,
            &Model::success
        );

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
                entity_name == other.entity_name &&
                entity_id == other.entity_id &&
                parameters == other.parameters &&
                request_body == other.request_body &&
                status_code == other.status_code &&
                error == other.error &&
                success == other.success;
        }
    };

    const AuthLog auth_log_from_crow_request(const crow::request& req, const std::string& body, int user_id,
                                             int status_code,
                                             int entity_id = 0, std::string error = "");
}
#undef Model
#undef MODEL
#undef COLS