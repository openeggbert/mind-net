//
// Created by robertvokac on 7/31/25.
//


#include "mindnet/controllers/RestHelper.h"

#include <string>
#include <crow.h>
#include <nlohmann/json.hpp>

#include "mindnet/Global.h"
#include "mindnet/Helper.h"
#include "mindnet/models/ModelDefinition.h"

namespace mindnet::controllers
{
    crow::json::wvalue RestHelper::model_to_wvalue(entity_fields& values, const models::ModelDefinition& def)
    {
        crow::json::wvalue res;
        auto columns = def.columns;

        for (int i = 0; i < columns.size(); i++)
        {
            auto column_name = columns[i].column_name;
            auto value = values[i];

            std::visit([&res, &column_name](auto&& val)
            {
                res[column_name] = crow::json::wvalue(val);
            }, values[i]);
        }
        return res;
    }

    str RestHelper::check_body_is_valid(const crow::json::rvalue& body, const models::ModelDefinition& def, const bool id_wanted)
    {
        for (auto e : def.columns)
        {
            std::cout << "Checking " << e.column_name << std::endl;
            if (e.column_name == "id")
            {
                if (!id_wanted && body.has(e.column_name))
                {
                    auto msg = "Id is not allowed in body";
                    err << msg << std::endl;
                    return msg;
                }
                if (id_wanted && !body.has(e.column_name))
                {
                    auto msg = "Mandatory column id is missing";
                    err << msg << std::endl;
                    return msg;
                }
                continue;
            }
            if (e.mandatory && !body.has(e.column_name))
            {
                auto msg = "Mandatory column " + e.column_name + " is missing";
                err << msg << std::endl;
                return msg;
            }
        }
        std::cout << "Body is valid" << std::endl;
        return "";
    }

#include <crow.h>
#include <nlohmann/json.hpp>

    nlohmann::json RestHelper::crow_json_to_nlohmann_json(const crow::json::rvalue& crow_json) {
        nlohmann::json result;

        for (const auto& key : crow_json.keys()) {
            const auto& val = crow_json[key];

            if (val.t() == crow::json::type::Null) {
                result[key] = nullptr;
            } else if (val.t() == crow::json::type::String) {
                result[key] = val.s();

            } else if (val.t() == crow::json::type::Number) {
                result[key] = val.d();
            } else if (val.t() == crow::json::type::True || val.t() == crow::json::type::False) {
                result[key] = val.b();
            } else if (val.t() == crow::json::type::List) {
                nlohmann::json arr = nlohmann::json::array();
                for (size_t i = 0; i < val.size(); ++i) {
                    arr.push_back(crow_json_to_nlohmann_json(val[i]));
                }
                result[key] = arr;
            } else if (val.t() == crow::json::type::Object) {
                result[key] = crow_json_to_nlohmann_json(val);
            }
        }

        return result;
    }

    crow::json::wvalue RestHelper::rjson_to_wjson(const crow::json::rvalue& rjson) {
        crow::json::wvalue wjson;

        for (const auto& key : rjson.keys()) {
            const auto& val = rjson[key];

            switch (val.t()) {
            case crow::json::type::Null:
                wjson[key] = crow::json::wvalue(); // default = null
                break;
            case crow::json::type::String:
                wjson[key] = val.s();
                break;
            case crow::json::type::Number:
                wjson[key] = val.d();
                break;
            case crow::json::type::True:
            case crow::json::type::False:
                wjson[key] = val.b();
                break;
            case crow::json::type::List: {
                    crow::json::wvalue::list list;
                    for (size_t i = 0; i < val.size(); ++i) {
                        list.push_back(rjson_to_wjson(val[i]));
                    }
                    wjson[key] = std::move(list);
                    break;
            }
            case crow::json::type::Object:
                wjson[key] = rjson_to_wjson(val);
                break;
            default: throw std::runtime_error("Unknown type");
            }

        }

        return wjson;
    }

}

