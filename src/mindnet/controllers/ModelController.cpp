//
// Created by robertvokac on 8/16/25.
//

#include "mindnet/controllers/ModelController.h"
#include "crow.h"
#include "mindnet/Utils.h"
#include "mindnet/controllers/RestHelper.h"

namespace mindnet::routes
{
    using controllers::RestHelper;

    void ModelController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db,
                                            models::ModelDefinition& def)
    {


        auto create_lambda_function = [&db, &def](const crow::request& req)
        {
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid input. Body is missing or not valid.");

            auto body_check_result = RestHelper::check_body_is_valid(body, def, false);
            if (!body_check_result.empty())
                return crow::response(400, "Invalid input. " + body_check_result);

            entity_fields fields = db->convert_crow_json_rvalue_to_entity_fields(body, def);

            auto last_inserted_id = db.get()->create(def, fields);
            if (last_inserted_id == -1)
            {
                return crow::response(500, "Saving the " + def.model_name + " failed.");
            }

            crow::json::wvalue res = RestHelper::rjson_to_wjson(body);
            res["id"] = last_inserted_id;

            return crow::response(200, res);
        };

        auto  read_lambda_function = [&db, &def](int id)
        {
            entity_fields values;
            try
            {
                values = db->read(id, def);
            }
            catch (std::runtime_error& e)
            {
                return crow::response(
                    404, "The " + def.model_name + " with id " + std::to_string(id) + " was not found.");
            }

            crow::json::wvalue res = RestHelper::model_to_wvalue(values, def);
            return crow::response(200, res);
        };


        //CREATE
        app.route_dynamic(str("/") + def.model_name).methods(crow::HTTPMethod::POST)
        (create_lambda_function);


        //READ
        app.route_dynamic(str("/") + def.model_name + "/<int>").methods(crow::HTTPMethod::GET)
        ( read_lambda_function);
    }
}
