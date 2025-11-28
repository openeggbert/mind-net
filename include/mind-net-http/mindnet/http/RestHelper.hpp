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
#include <crow.h>
#include <nlohmann/json.hpp>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ModelDefinition.hpp"

namespace mindnet::http
{
    class RestHelper
    {
    private:
        RestHelper() = delete;
        RestHelper(const RestHelper&) = delete;
        RestHelper& operator=(const RestHelper&) = delete;

    public:
        static crow::json::wvalue model_to_wvalue(
            const entity_fields& values,
            const model::ModelDefinition& def,
            const std::set<std::string>& fields_vector_filter);

        static string check_body_is_valid(const crow::json::rvalue& body, const model::ModelDefinition& def,
                                          bool id_wanted = true);

        static nlohmann::json crow_json_to_nlohmann_json(const crow::json::rvalue& crow_json);

        static crow::json::wvalue rjson_to_wjson(const crow::json::rvalue& rjson);
    };
}