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
            auto column_name = columns[i].first;
            auto value = values[i];

            std::visit([&res, &column_name](auto&& val)
            {
                res[column_name] = crow::json::wvalue(val);
            }, values[i]);
        }
        return res;
    }


    void ContentController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db,
                                            models::ModelDefinition& def)
    {
        typedef models::columns::ContentColumns cols;

        //CREATE
        app.route_dynamic(str("/") + def.model_name).methods(crow::HTTPMethod::POST)
        ([db](const crow::request& req)
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has(cols::CONTENT) || !body.has(cols::FORMAT))
                return crow::response(400, "Invalid input");

            models::Content c{
                0, body[cols::CONTENT].s(), body[cols::FORMAT].s(), static_cast<int>(Utils::currentUnixTimestamp())
            };
            auto last_inserted_id = db.get()->content_repository->create(c);

            crow::json::wvalue res;
            res["id"] = last_inserted_id;
            res["content"] = c.content;
            res["format"] = c.format;
            res["created_at"] = c.created_at;
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
