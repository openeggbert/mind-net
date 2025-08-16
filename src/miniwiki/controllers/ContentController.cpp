//
// Created by robertvokac on 8/16/25.
//

#include "miniwiki/controllers/ContentController.h"
#include "crow.h"
#include "miniwiki/Utils.h"
#include "miniwiki/models/Content.h"

namespace miniwiki::routes
{
    crow::json::wvalue model_to_wvalue(entity_fields& values, const models::ModelDefinition& def)
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

    str check_body_is_valid(crow::json::rvalue body, models::ModelDefinition& def, bool id_wanted = true)
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

    nlohmann::json crow_json_to_nlohmann_json(const crow::json::rvalue& crow_json) {
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

    crow::json::wvalue rjson_to_wjson(const crow::json::rvalue& rjson) {
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


    void ContentController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db,
                                            models::ModelDefinition& def)
    {
        typedef models::columns::ContentColumns cols;

        //CREATE
        app.route_dynamic(str("/") + def.model_name).methods(crow::HTTPMethod::POST)
        ([&db, &def](const crow::request& req)
        {
            auto body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid input. Body is missing or not valid.");

            auto body_check_result = check_body_is_valid(body, def, false);
            if (!body_check_result.empty())
                return crow::response(400, "Invalid input. " + body_check_result);

            models::Content c{
                0, body[cols::CONTENT].s(), body[cols::FORMAT].s(), static_cast<int>(Utils::currentUnixTimestamp())
            };
            auto last_inserted_id = db.get()->content_repository->create(c);
            if (last_inserted_id == -1)
            {
                return crow::response(500, "Saving the content failed.");
            }

            crow::json::wvalue res = rjson_to_wjson(body);
            res["id"] = last_inserted_id;

            return crow::response(200, res);
        });

        //READ
        app.route_dynamic(str("/") + def.model_name + "/<int>").methods(crow::HTTPMethod::GET)
        ([&db, &def](int id)
        {
            entity_fields values;
            try
            {
                values = db->content_repository->read(id).get_values();
            }
            catch (std::runtime_error& e)
            {
                return crow::response(
                    404, "The " + def.model_name + " with id " + std::to_string(id) + " was not found.");
            }

            crow::json::wvalue res = model_to_wvalue(values, def);
            return crow::response(200, res);
        });
    }
}
