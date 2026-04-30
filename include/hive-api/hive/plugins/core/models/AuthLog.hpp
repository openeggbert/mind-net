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
#include "hive/model/BaseModel.hpp"
#include "hive/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model AuthLog
#define MODEL AUTH_LOG
#define COLS columns::AuthLogColumns
#include "../columns/AuthLogColumns.hpp"
#include "crow/http_request.h"
#include "hive/plugins/core/enums/HttpMethod.hpp"

// ***** MACROS : END *****

namespace hive::plugins::core::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();
    inline const def AUTH_LOG_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_title_column(COLS::ENDPOINT)
        .set_group("Core", 10000).allow_reader_write().set_readonly()
        .set_columns({
            //
            coldef(COLS::USER_ID, FOREIGN_KEY).set_description("User ID who performed the request"),
            coldef(COLS::IP_ADDRESS).set_description("Client IP address"),
            coldef(COLS::USER_AGENT).set_description("Client user agent string"),
            coldef(COLS::ENDPOINT, MANDATORY).set_description("Requested API endpoint"),
            coldef(COLS::METHOD, MANDATORY).set_enum_definition(
                enums::http_method_to_enum_definition()).set_description("HTTP method"),
            coldef(COLS::ACTION).set_description("Action performed"),
            coldef(COLS::ENTITY_NAME, MANDATORY).set_description("Target model name"),
            coldef(COLS::ENTITY_ID).set_description("Target entity ID"),
            coldef(COLS::PARAMETERS).set_description("Query parameters"),
            coldef(COLS::REQUEST_BODY).set_description("Request body"),
            coldef(COLS::STATUS_CODE, INTEGER | MANDATORY).set_description("HTTP response status code"),
            coldef(COLS::ERROR).set_description("Error message if request failed"),
            coldef(COLS::SUCCESS, BOOL).set_description("Whether the request succeeded")
            //
        });

    struct Model : hive::model::BaseModel
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

    };

    const AuthLog auth_log_from_crow_request(const crow::request& req, const std::string& body, int user_id,
                                             int status_code,
                                             int entity_id = 0, std::string error = "");
}
#undef Model
#undef MODEL
#undef COLS