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
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_title_column(COLS::ENDPOINT)
        .set_group("Core", 10000).allow_reader_write().set_readonly()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY).set_description("User ID who performed the request"),
            coldef(COLS::IP_ADDRESS).set_description("Client IP address"),
            coldef(COLS::USER_AGENT).set_description("Client user agent string"),
            coldef(COLS::ENDPOINT, MANDATORY).set_description("Requested API endpoint"),
            coldef(COLS::METHOD, MANDATORY).set_enum_definition(
                enums::http_method_to_enum_definition()).set_description("HTTP method"),
            coldef(COLS::ACTION, MANDATORY).set_description("Action performed"),
            coldef(COLS::PARAMETERS).set_description("Query parameters"),
            coldef(COLS::REQUEST_BODY).set_description("Request body"),
            coldef(COLS::DIFF).set_description("Differences between change"),
            coldef(COLS::STATUS_CODE, INTEGER | MANDATORY).set_description("HTTP response status code"),
            coldef(COLS::ERROR).set_description("Error message if request failed"),
            coldef(COLS::SUCCESS, BOOL).set_description("Whether the request succeeded")
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
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

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::ip_address,
            &Model::user_agent,
            &Model::endpoint,
            &Model::method,
            &Model::action,
            &Model::parameters,
            &Model::request_body,
            &Model::diff,
            &Model::status_code,
            &Model::error,
            &Model::success
        );

        create_model_h_methods(Model, MODEL)

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