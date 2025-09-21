//
// Created by robertvokac on 8/16/25.
//

#include "mindnet/http/ModelEndpointGenerator.h"
#include "crow.h"
#include "mindnet/Configuration.h"
#include "mindnet/Utils.h"
#include "mindnet/http/RestHelper.h"
#include "mindnet/impl/sqlite/RepositoryHelper.h"
#include "mindnet/plugins/core/models/ApiLog.h"

#define check_maintenance_mode()\
if (g_configuration.access_mode == AccessMode::MaintenanceMode)\
return crow::response(503, "Maintenance Mode. Service Unavailable.");

namespace mindnet::http
{
    using http::RestHelper;
    using plugins::core::enums::Crudl;

    void ModelEndpointGenerator::create_model_endpoint(
        crow::SimpleApp& app,
        ServicePtr& service_ptr,
        ModelDefinition& def)
    {
        auto split_string_by_commas = [](const string& string_, std::set<std::string>& result)
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
        auto log_request = [] (const ServicePtr& service_ptr, const crow::request& req, http::LoginToken& login_token, int status_code, int entity_id = 0, const std::string& error = "")
        {
            auto log_object = plugins::core::models::api_log_from_crow_request(
         req,
         login_token.user_id,
         status_code,
         entity_id,
         error);
            auto log = log_object.to_values();
            int64_t now = static_cast<int64_t>(Utils::currentUnixTimestamp());
            log[1] = now;
            log[2] = now;

            if (service_ptr == nullptr)
            {
                warn << "Saving record to the table log failed for this reason: service_ptr == nullptr " << log_object.to_json() << commit;
                return;
            }

            auto result = service_ptr->create(
                plugins::core::models::API_LOG_DEFINITION,
                login_token,
                log)
                ;
            if (result.second.ko())
            {
                warn << "Saving record to the table log failed for this reason: " << result.second.error << log_object.to_json() << commit;
            }
        };
        auto create_lambda_function = [&service_ptr, &def, log_request](const crow::request& req)
        {
            check_maintenance_mode()

            trace << "Create lambda function called" << commit;
            http::LoginToken login_token{req};
            if (!def.get_allowed_rest_operations().contains(Crudl::Create)){
                log_request(service_ptr, req, login_token, 405, 0, "Method not allowed for model " + def.get_model_name() + ".");
                return crow::response(405, "Method not allowed for model " + def.get_model_name() + ".");
            }
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body) {
                log_request(service_ptr, req, login_token, 400, 0, "Invalid input. Body is missing or not valid.");
                return crow::response(400, "Invalid input. Body is missing or not valid.");
            }

            auto body_check_result = RestHelper::check_body_is_valid(body, def, false);
            if (!body_check_result.empty())
            {
                log_request(service_ptr, req, login_token, 400, 0, "Invalid input. " + body_check_result);
                return crow::response(400, "Invalid input. " + body_check_result);
            }

            entity_fields fields = service_ptr->request_to_entity_fields(body, Crudl::Create, def);
            if (fields.size() != def.get_columns().size())
            {
                log_request(service_ptr, req, login_token, 500, 0, "Server issue. Function request_to_entity_fields() is not correctly implemented for model "
                    + def.get_model_name() + ".");
                return crow::response(
                    500,
                    "Server issue. Function request_to_entity_fields() is not correctly implemented for model "
                    + def.get_model_name() + ".");
            }

            auto last_inserted_id = service_ptr.get()->create(def, login_token, fields);
            if (last_inserted_id.first == -1)
            {
                log_request(service_ptr, req, login_token, 500, 0, "Saving the " + def.get_model_name() + " failed. Error: " + last_inserted_id.second.error);
                return crow::response(
                    500, "Saving the " + def.get_model_name() + " failed. Error: " + last_inserted_id.second.error);
            }

            crow::json::wvalue res = RestHelper::rjson_to_wjson(body);
            res["id"] = last_inserted_id.first;

            log_request(service_ptr, req, login_token, 200, last_inserted_id.first);
            return crow::response(200, res);
        };

        auto read_lambda_function = [&service_ptr, &def, &split_string_by_commas, &log_request](const crow::request& req, int id)
        {
            check_maintenance_mode()

            trace << "Read lambda function called" << commit;

            http::LoginToken login_token{req};

            if (!def.get_allowed_rest_operations().contains(Crudl::Read))
            {
                log_request(service_ptr, req, login_token, 405, id, "Method not allowed for model " + def.get_model_name() + ".");
                return crow::response(405, "Method not allowed for model " + def.get_model_name() + ".");
            }
            entity_fields values;

            string error;
            try
            {
                auto read_result = service_ptr->read(def, login_token, id);
                if (read_result.second.ko())
                {
                    error = read_result.second.error;
                }
                else
                {
                    values = read_result.first;
                }
            }
            catch (std::runtime_error& e)
            {
                log_request(service_ptr, req, login_token, 404, id, "The " + def.get_model_name() + " with id " + std::to_string(id) + " was not found. " + error);
                return crow::response(
                    404, "The " + def.get_model_name() + " with id " + std::to_string(id) + " was not found. " + error);
            }
            if (!error.empty())
            {
                log_request(service_ptr, req, login_token, 404, id, "You cannot read the model. " + error);
                return crow::response(404, "You cannot read the model. " + error);
            }

            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";
            std::set<std::string> fields_set_filter{};
            split_string_by_commas(fields, fields_set_filter);

            crow::json::wvalue res = RestHelper::model_to_wvalue(values, def, fields_set_filter);
            log_request(service_ptr, req, login_token, 200, id);
            return crow::response(200, res);
        };

        auto update_lambda_function = [&service_ptr, &def, &log_request](const crow::request& req, int id)
        {
            check_maintenance_mode()

            trace << "Update lambda function called" << commit;
            http::LoginToken login_token{req};

            if (!def.get_allowed_rest_operations().contains(Crudl::Update))
            {
                log_request(service_ptr, req, login_token, 405, id, "Method not allowed for model " + def.get_model_name() + ".");
                return crow::response(405, "Method not allowed for model " + def.get_model_name() + ".");
            }
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body)
            {
                log_request(service_ptr, req, login_token, 400, id, "Invalid input. Body is missing or not valid.");
                return crow::response(400, "Invalid input. Body is missing or not valid.");
            }

            auto body_check_result = RestHelper::check_body_is_valid(body, def, true); // true = allow partial update?
            if (!body_check_result.empty())
            {
                log_request(service_ptr, req, login_token, 400, id, "Invalid input. " + body_check_result);
                return crow::response(400, "Invalid input. " + body_check_result);
            }

            if (id != body["id"].i())
            {
                log_request(service_ptr, req, login_token, 400, id, "Invalid input. id in body is not equal to id in url.");
                return crow::response(400, "Invalid input. id in body is not equal to id in url.");
            }
            entity_fields fields = service_ptr->request_to_entity_fields(body, Crudl::Update, def);
            if (fields.size() != def.get_columns().size())
            {
                log_request(service_ptr, req, login_token, 500, id, "Server issue. Function request_to_entity_fields() is not correctly implemented for model "
                    + def.get_model_name() + ".");
                return crow::response(
                    500,
                    "Server issue. Function request_to_entity_fields() is not correctly implemented for model "
                    + def.get_model_name() + ".");
            }

            auto success = service_ptr->update(def, login_token, id, fields);
            if (success.ko())
            {
                log_request(service_ptr, req, login_token, 404, id, "Update failed. " + def.get_model_name() + " with id " + std::to_string(id) + " not found. " +
                    success.error);

                return crow::response(
                    404, "Update failed. " + def.get_model_name() + " with id " + std::to_string(id) + " not found. " +
                    success.error);
            }

            crow::json::wvalue res = RestHelper::rjson_to_wjson(body);
            res["id"] = id;

            log_request(service_ptr, req, login_token, 200, id);
            return crow::response(200, res);
        };

        auto delete_lambda_function = [&service_ptr, &def, &log_request](const crow::request& req, int id)
        {
            check_maintenance_mode()

            trace << "Delete lambda function called" << commit;
            http::LoginToken login_token{req};
            if (!def.get_allowed_rest_operations().contains(Crudl::Delete))
            {
                log_request(service_ptr, req, login_token, 405, id, "Method not allowed for model " + def.get_model_name() + ".");
                return crow::response(405, "Method not allowed for model " + def.get_model_name() + ".");
            }

            auto success = service_ptr->remove(def, login_token, id);

            if (success.ko())
            {
                log_request(service_ptr, req, login_token, 404, id, "Delete failed. " + def.get_model_name() + " with id " + std::to_string(id) + " not found. " +
                    success.error);
                return crow::response(
                    404, "Delete failed. " + def.get_model_name() + " with id " + std::to_string(id) + " not found. " +
                    success.error);
            }

            log_request(service_ptr, req, login_token, 200, id);
            return crow::response(200, def.get_model_name() + " with id " + std::to_string(id) + " was deleted.");
        };

        auto list_lambda_function = [&service_ptr, &def, &split_string_by_commas, &log_request](const crow::request& req)
        {
            check_maintenance_mode()

            trace << "List lambda function called" << commit;
            http::LoginToken login_token{req};

            if (!def.get_allowed_rest_operations().contains(Crudl::List))
            {
                log_request(service_ptr, req, login_token, 405, 0, "Method not allowed for model " + def.get_model_name() + ".");
                return crow::response(405, "Method not allowed for model " + def.get_model_name() + ".");
            }
            int page_number = req.url_params.get("page_number") ? std::stoi(req.url_params.get("page_number")) : 1;
            int page_size = req.url_params.get("page_size") ? std::stoi(req.url_params.get("page_size")) : 20;
            if (page_number <= 0)
            {
                log_request(service_ptr, req, login_token, 400, 0, "Invalid page number. It must be positive");
                return crow::response(400, "Invalid page number. It must be positive");
            }
            if (page_size < 5)
            {
                log_request(service_ptr, req, login_token, 400, 0, "Invalid page size. It must be 5 at least");
                return crow::response(400, "Invalid page size. It must be 5 at least");
            }
            if (page_size > 100)
            {
                log_request(service_ptr, req, login_token, 400, 0, "Invalid page size. It must be 100 at most");
                return crow::response(400, "Invalid page size. It must be 100 at most");
            }
            string sort = req.url_params.get("sort") ? req.url_params.get("sort") : "";
            string order = req.url_params.get("order") ? req.url_params.get("order") : "";
            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            http::QueryParams query_params;
            query_params.page_number = page_number;
            query_params.page_size = page_size;
            query_params.total_items = 0;
            query_params.sort = sort;
            query_params.order = order.empty() ? http::Order::Asc : http::string_to_order(order);


            split_string_by_commas(fields, query_params.fields);

            for (auto& f : query_params.fields)
            {
                experiment << "field: " << f << commit;
            }

            for (const auto& column : def.get_columns())
            {
                if (!req.url_params.get(column.get_column_name()))
                {
                    continue;
                }
                if (column.get_column_name() == "id")
                {
                    continue;
                    //return crow::response(500, "Failed to list " + def.get_model_name() + " records. " + "Error: Parameters contain id.");
                }
                string value = req.url_params.get(column.get_column_name());
                query_params.add_filter(column.get_column_name(), value);
            }

            auto all_records = service_ptr->list(def, login_token, query_params);
            if (all_records.second.ko())
            {
                log_request(service_ptr, req, login_token, 500, 0, "Failed to list " + def.get_model_name() + " records. " + "Error: " + all_records.second.
                    error);
                return crow::response(
                    500, "Failed to list " + def.get_model_name() + " records. " + "Error: " + all_records.second.
                    error);
            }

            crow::json::wvalue res;
            std::vector<crow::json::wvalue> items;

            for (const auto& record : all_records.first)
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

            log_request(service_ptr, req, login_token, 200);
            return crow::response(200, res);
        };

        //CREATE
        app.route_dynamic(string("/api/v1/") + def.get_model_name()).methods(crow::HTTPMethod::POST)
            (create_lambda_function);


        //READ
        app.route_dynamic(string("/api/v1/") + def.get_model_name() + "/<int>").methods(crow::HTTPMethod::GET)
            (read_lambda_function);

        // UPDATE
        app.route_dynamic(string("/api/v1/") + def.get_model_name() + "/<int>").methods(crow::HTTPMethod::PUT)
            (update_lambda_function);

        // DELETE
        app.route_dynamic(string("/api/v1/") + def.get_model_name() + "/<int>").methods(crow::HTTPMethod::DELETE)
            (delete_lambda_function);

        // LIST
        app.route_dynamic(string("/api/v1/") + def.get_model_name()).methods(crow::HTTPMethod::GET)
            (list_lambda_function);
    }
}
