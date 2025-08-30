//
// Created by robertvokac on 8/16/25.
//

#include "mindnet/controllers/ModelController.h"
#include "crow.h"
#include "mindnet/Utils.h"
#include "mindnet/controllers/RestHelper.h"
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"

namespace mindnet::routes
{
    using controllers::RestHelper;
    using enums::Crudl;

    void ModelController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db,
                                          models::misc::ModelDefinition& def)
    {
        auto split_string_by_commas = [](const str& string_, std::set<std::string>& result)
        {
            if (!string_.empty())
            {
                std::stringstream ss(string_);
                std::string field_entry;

                while (std::getline(ss, field_entry, ','))
                {
                    result.insert(field_entry);
                }
            }
        };
        auto create_lambda_function = [&db, &def](const crow::request& req)
        {
            trace << "Create lambda function called" << commit;
            if (!def.allowed_rest_operations.empty() && !def.allowed_rest_operations.
                                                             contains(Crudl::CREATE))
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
                return crow::response(
                    500,
                    "Server issue. Function convert_crow_json_rvalue_to_entity_fields() is not correctly implemented for model "
                    + def.model_name + ".");
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

        auto read_lambda_function = [&db, &def, &split_string_by_commas](const crow::request& req, int id)
        {
            trace << "Read lambda function called" << commit;
            if (!def.allowed_rest_operations.empty() && !def.allowed_rest_operations.contains(Crudl::READ))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            entity_fields values;
            str error;
            try
            {
                values = db->read(id, def, error);
            }
            catch (std::runtime_error& e)
            {
                return crow::response(
                    404, "The " + def.model_name + " with id " + std::to_string(id) + " was not found. " + error);
            }

            str fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";
            std::set<std::string> fields_set_filter{};
            split_string_by_commas(fields, fields_set_filter);

            crow::json::wvalue res = RestHelper::model_to_wvalue(values, def, fields_set_filter);
            return crow::response(200, res);
        };

        auto update_lambda_function = [&db, &def](const crow::request& req, int id)
        {
            trace << "Update lambda function called" << commit;
            if (!def.allowed_rest_operations.empty() && !def.allowed_rest_operations.
                                                             contains(Crudl::UPDATE))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid input. Body is missing or not valid.");

            auto body_check_result = RestHelper::check_body_is_valid(body, def, true); // true = allow partial update?
            if (!body_check_result.empty())
                return crow::response(400, "Invalid input. " + body_check_result);

            if (id != body["id"].i())
            {
                return crow::response(400, "Invalid input. id in body is not equal to id in url.");
            }
            entity_fields fields = db->convert_crow_json_rvalue_to_entity_fields(body, enums::Crudl::UPDATE, def);
            if (fields.size() != def.columns.size())
            {
                return crow::response(
                    500,
                    "Server issue. Function convert_crow_json_rvalue_to_entity_fields() is not correctly implemented for model "
                    + def.model_name + ".");
            }

            str error;
            bool success = db->update(id, fields, def, error);
            if (!success)
            {
                return crow::response(
                    404, "Update failed. " + def.model_name + " with id " + std::to_string(id) + " not found. " +
                    error);
            }

            crow::json::wvalue res = RestHelper::rjson_to_wjson(body);
            res["id"] = id;

            return crow::response(200, res);
        };

        auto delete_lambda_function = [&db, &def](int id)
        {
            trace << "Delete lambda function called" << commit;
            if (!def.allowed_rest_operations.empty() && !def.allowed_rest_operations.
                                                             contains(Crudl::DELETE))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            str error;
            bool success = db->remove(id, def, error);

            if (!success)
            {
                return crow::response(
                    404, "Delete failed. " + def.model_name + " with id " + std::to_string(id) + " not found. " +
                    error);
            }

            return crow::response(200, def.model_name + " with id " + std::to_string(id) + " was deleted.");
        };

        auto list_lambda_function = [&db, &def, &split_string_by_commas](const crow::request& req)
        {
            trace << "List lambda function called" << commit;
            if (!def.allowed_rest_operations.empty() && !def.allowed_rest_operations.contains(Crudl::LIST))
                return crow::response(405, "Method not allowed for model " + def.model_name + ".");
            int page_number = req.url_params.get("page_number") ? std::stoi(req.url_params.get("page_number")) : 1;
            int page_size = req.url_params.get("page_size") ? std::stoi(req.url_params.get("page_size")) : 20;
            if (page_number <= 0)
            {
                return crow::response(400, "Invalid page number. It must be positive");
            }
            if (page_size < 5)
            {
                return crow::response(400, "Invalid page size. It must be 5 at least");
            }
            if (page_size > 100)
            {
                return crow::response(400, "Invalid page size. It must be 100 at most");
            }
            str sort = req.url_params.get("sort") ? req.url_params.get("sort") : "";
            str order = req.url_params.get("order") ? req.url_params.get("order") : "";
            str fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::vector<entity_fields> all_records;

            http::QueryParams query_params;
            query_params.page_number = page_number;
            query_params.page_size = page_size;
            query_params.total_items = 0;
            query_params.sort = sort;
            query_params.order = order.empty() ? http::Order::ASC : http::string_to_order(order);


            split_string_by_commas(fields, query_params.fields);

            for (auto& f : query_params.fields)
            {
                test << "field: " << f << commit;
            }
            str error;
            for (const auto& column : def.columns)
            {
                if (!req.url_params.get(column.column_name))
                {
                    continue;
                }
                str value = req.url_params.get(column.column_name);
                query_params.add_filter(column.column_name, value);
            }

            all_records = db->list(query_params, def, error);
            if (!error.empty())
            {
                return crow::response(500, "Failed to list " + def.model_name + " records. " + "Error: " + error);
            }

            crow::json::wvalue res;
            std::vector<crow::json::wvalue> items;

            for (const auto& record : all_records)
            {
                items.push_back(RestHelper::model_to_wvalue(record, def, query_params.fields));
            }

            res["items"] = std::move(items);
            res["total_items"] = query_params.total_items;
            res["page_number"] = page_number;
            res["page_size"] = page_size;
            int total_pages = query_params.total_items / page_size;
            if (query_params.total_items % page_size != 0)
            {
                ++total_pages;
            }
            res["total_pages"] = total_pages;

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
