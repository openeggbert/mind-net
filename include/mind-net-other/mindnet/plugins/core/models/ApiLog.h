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
#ifndef APILOG_H
#define APILOG_H

#include <string>
#include "../../../model/BaseModel.h"

// ***** MACROS : START *****
#define Model ApiLog
#define MODEL API_LOG
#define COLS columns::ApiLogColumns
#include "../columns/ApiLogColumns.h"
#include "crow/http_request.h"
#include "mindnet/plugins/core/enums/HttpMethod.h"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline def API_LOG_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("rl")
        .set_title_column(COLS::ENDPOINT)
        .set_group("Core", 600).allow_reader_write()
        .set_columns({
            //
            coldef(COLS::USER_ID, FOREIGN_KEY | READONLY).set_description("User ID who performed the request"),
            coldef(COLS::IP_ADDRESS, READONLY).set_description("Client IP address"),
            coldef(COLS::ENDPOINT, MANDATORY | READONLY).set_description("Requested API endpoint"),
            coldef(COLS::METHOD, MANDATORY | READONLY).set_enum_definition(
                enums::http_method_to_enum_definition()).set_description("HTTP method"),
            coldef(COLS::ENTITY_NAME, MANDATORY | READONLY).set_description("Target model name"),
            coldef(COLS::ENTITY_ID, READONLY).set_description("Target entity ID"),
            coldef(COLS::PARAMETERS, READONLY).set_description("Query parameters"),
            coldef(COLS::REQUEST_BODY, READONLY).set_description("Request body"),
            coldef(COLS::STATUS_CODE, INTEGER | MANDATORY | READONLY).set_description("HTTP response status code"),
            coldef(COLS::ERROR, READONLY).set_description("Error message if request failed")
            //
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        std::string ip_address;
        std::string endpoint;
        enums::HttpMethod method{enums::HttpMethod::Undefined};
        std::string entity_name;
        int entity_id{};
        std::string parameters;
        std::string request_body;
        int status_code{};
        std::string error;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                   created_at == other.created_at &&
                   updated_at == other.updated_at &&
                   user_id == other.user_id &&
                   ip_address == other.ip_address &&
                   endpoint == other.endpoint &&
                   method == other.method &&
                   entity_name == other.entity_name &&
                   entity_id == other.entity_id &&
                   parameters == other.parameters &&
                   request_body == other.request_body &&
                   status_code == other.status_code &&
                   error == other.error;
        }
    };


    inline std::string serializeParams(const crow::query_string& qs)
    {
        std::ostringstream oss;
        bool first = true;
        for (auto& key: qs.keys())
        {
            auto value = qs.get(key);
            if (!first) oss << "&";
            oss << key << "=" << value;
            first = false;
        }
        return oss.str();
    }

    inline const ApiLog api_log_from_crow_request(const crow::request& req, int user_id, int status_code, int entity_id = 0, std::string error = "")
    {
        ApiLog log;

        log.user_id = user_id;
        log.ip_address = req.remote_ip_address;
        log.endpoint = req.url; // optionally just path without query

        // method mapping
        switch (req.method)
        {
        case crow::HTTPMethod::Get: log.method = enums::HttpMethod::GET;
            break;
        case crow::HTTPMethod::Post: log.method = enums::HttpMethod::POST;
            break;
        case crow::HTTPMethod::Put: log.method = enums::HttpMethod::PUT;
            break;
        case crow::HTTPMethod::Delete: log.method = enums::HttpMethod::DELETE;
            break;
        case crow::HTTPMethod::Patch: log.method = enums::HttpMethod::PATCH;
            break;
        default: log.method = enums::HttpMethod::Undefined;
            break;
        }


        // Entity name + ID - if it's part of the URL
        // e.g. /api/v1/note/123  → entity_name="note", entity_id=123
        {
            std::string path = req.url;
            // very simple parsing, regex or router metadata would be better in practice
            std::istringstream iss(path);
            std::string segment;
            std::vector<std::string> parts;
            while (std::getline(iss, segment, '/'))
            {
                if (!segment.empty()) parts.push_back(segment);
            }
            if (parts.size() >= 3 && parts[0] == "api" && parts[1].rfind("v", 0) == 0)
            {
                log.entity_name = parts[2];
                if (parts.size() >= 4)
                {
                    try { log.entity_id = std::stoi(parts[3]); }
                    catch (...) { log.entity_id = 0; }
                }
            }
            if (log.entity_id == 0 && entity_id != 0)
            {
                log.entity_id = entity_id;
            }
        }

        log.parameters = serializeParams(req.url_params);
        log.request_body = req.body;
        log.status_code = status_code;
        log.error = error;

        return log;
    }
}
#undef Model
#undef MODEL
#undef COLS

#endif // APILOG_H
