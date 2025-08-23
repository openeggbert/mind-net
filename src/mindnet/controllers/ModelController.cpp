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
    using enums::Crudl;
    void ModelController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db,
                                          models::misc::ModelDefinition& def)
    {
        auto create_lambda_function = [&db, &def](const crow::request& req)
        {
            if (!def.allowed_crudl_rest_operations.empty() && !def.allowed_crudl_rest_operations.contains(Crudl::CREATE))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid input. Body is missing or not valid.");

            auto body_check_result = RestHelper::check_body_is_valid(body, def, false);
            if (!body_check_result.empty())
                return crow::response(400, "Invalid input. " + body_check_result);

            entity_fields fields = db->convert_crow_json_rvalue_to_entity_fields(body, enums::Crudl::CREATE, def);
            if (fields.size() != def.columns.size())
            {
                return crow::response(500, "Server issue. Function convert_crow_json_rvalue_to_entity_fields() is not correctly implemented for model " + def.model_name+ ".");
            }

            str error;
            auto last_inserted_id = db.get()->create(def, fields, error);
            if (last_inserted_id == -1)
            {
                return crow::response(500, "Saving the " + def.model_name + " failed. Error: " + error);
            }

            crow::json::wvalue res = RestHelper::rjson_to_wjson(body);
            res["id"] = last_inserted_id;

            return crow::response(200, res);
        };

        auto read_lambda_function = [&db, &def](int id)
        {
            if (!def.allowed_crudl_rest_operations.empty() && !def.allowed_crudl_rest_operations.contains(Crudl::READ))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
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

        auto update_lambda_function = [&db, &def](const crow::request& req, int id)
        {
            std::cout << "Update lambda function called" << std::endl;
            if (!def.allowed_crudl_rest_operations.empty() && !def.allowed_crudl_rest_operations.contains(Crudl::UPDATE))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid input. Body is missing or not valid.");

            auto body_check_result = RestHelper::check_body_is_valid(body, def, true); // true = allow partial update?
            if (!body_check_result.empty())
                return crow::response(400, "Invalid input. " + body_check_result);

            entity_fields fields = db->convert_crow_json_rvalue_to_entity_fields(body, enums::Crudl::UPDATE, def);
            if (fields.size() != def.columns.size())
            {
                return crow::response(500, "Server issue. Function convert_crow_json_rvalue_to_entity_fields() is not correctly implemented for model " + def.model_name+ ".");
            }

            bool success = db->update(id, fields, def);
            if (!success)
            {
                return crow::response(
                    404, "Update failed. " + def.model_name + " with id " + std::to_string(id) + " not found.");
            }

            crow::json::wvalue res = RestHelper::rjson_to_wjson(body);
            res["id"] = id;

            return crow::response(200, res);
        };

        auto delete_lambda_function = [&db, &def](int id)
        {
            if (!def.allowed_crudl_rest_operations.empty() && !def.allowed_crudl_rest_operations.contains(Crudl::DELETE))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            bool success = db->remove(id, def);

            if (!success)
            {
                return crow::response(
                    404, "Delete failed. " + def.model_name + " with id " + std::to_string(id) + " not found.");
            }

            return crow::response(200, def.model_name + " with id " + std::to_string(id) + " was deleted.");
        };

        auto list_lambda_function = [&db, &def](const crow::request& req)
        {
            std::cout << "List lambda function called" << std::endl;
            if (!def.allowed_crudl_rest_operations.empty() && !def.allowed_crudl_rest_operations.contains(Crudl::LIST))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            int page_number = req.url_params.get("page_number") ? std::stoi(req.url_params.get("page_number")) : 1;
            int page_size = req.url_params.get("page_size") ? std::stoi(req.url_params.get("page_size")) : 20;
            if (page_number <= 0)
            {
                return crow::response(400, "Invalid page number. It must be positive");
            }
            if (page_size <= 0)
            {
                return crow::response(400, "Invalid page size. It must be positive");
            }

            std::vector<entity_fields> all_records;
            try
            {
                all_records = db->list(page_number, page_size, def);
            }
            catch (std::runtime_error& e)
            {
                return crow::response(500, "Failed to list " + def.model_name + " records.");
            }

            crow::json::wvalue res;
            std::vector<crow::json::wvalue> items;

            for (const auto& record : all_records)
            {
                items.push_back(RestHelper::model_to_wvalue(record, def));
            }

            res["items"] = std::move(items);

            return crow::response(200, res);
        };


        //CREATE
        app.route_dynamic(str("/api/") + def.model_name).methods(crow::HTTPMethod::POST)
            (create_lambda_function);


        //READ
        app.route_dynamic(str("/api/") + def.model_name + "/<int>").methods(crow::HTTPMethod::GET)
            (read_lambda_function);

        // UPDATE
        app.route_dynamic(str("/api/") + def.model_name + "/<int>").methods(crow::HTTPMethod::PUT)
            (update_lambda_function);

        // DELETE
        app.route_dynamic(str("/api/") + def.model_name + "/<int>").methods(crow::HTTPMethod::DELETE)
            (delete_lambda_function);

        // LIST
        app.route_dynamic(str("/api/") + def.model_name).methods(crow::HTTPMethod::GET)
            (list_lambda_function);
    }
}
