//
// Created by robertvokac on 7/31/25.
//
#ifndef RESTHELPER_H
#define RESTHELPER_H

#include <string>
#include <crow.h>
#include <nlohmann/json.hpp>

#include "mindnet/Helper.h"
#include "mindnet/models/ModelDefinition.h"

namespace mindnet::controllers
{
    class RestHelper
    {
    private:
        RestHelper() = delete;
        RestHelper(const RestHelper&) = delete;
        RestHelper& operator=(const RestHelper&) = delete;

    public:
        static crow::json::wvalue model_to_wvalue(entity_fields& values, const models::ModelDefinition& def);

        static str check_body_is_valid(const crow::json::rvalue& body, const models::ModelDefinition& def,
                                       bool id_wanted = true);

        static nlohmann::json crow_json_to_nlohmann_json(const crow::json::rvalue& crow_json);

        static crow::json::wvalue rjson_to_wjson(const crow::json::rvalue& rjson);

    };
}

#endif // RESTHELPER_H
