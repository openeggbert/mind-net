//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/ModelDefinitionEndpointsGenerator.hpp"

#include "mindnet/api/IService.hpp"
#include "mindnet/essential/Configuration.hpp"
#include "mindnet/http/HttpUtils.hpp"
#include "mindnet/plugins/core/models/User.hpp"

namespace mindnet::http
{
    void ModelDefinitionEndpointsGenerator::create_model_definition_endpoints(
        const api::ServicePtr& service_ptr,
        crow::SimpleApp& crow_app
    )
    {
        auto column_definition_to_json = [](mindnet::model::ColumnDefinition& column_definition)
        {
            crow::json::wvalue result;
            // if (column_definition.is_hidden())
            // {
            //     return result;
            // }

            result["column_name"] = column_definition.get_column_name();
            result["column_type"] = column_type_to_string(column_definition.get_column_type());
            result["mandatory"] = column_definition.is_mandatory();
            result["primary_key"] = column_definition.is_primary_key();
            result["foreign_key"] = column_definition.get_foreign_key();
            if (column_definition.get_enum_definition().has_value())
            {
                auto enum_ = column_definition.get_enum_definition().value();
                crow::json::wvalue::list enum_list;
                int enum_size = enum_.get_value_count();
                for (int i = 0; i < enum_size; ++i)
                {
                    int number = enum_.get_value(i);
                    string text = enum_.get_value_as_string(i);
                    crow::json::wvalue enum_json;
                    enum_json[text] = number;
                    enum_list.push_back(std::move(enum_json));
                }

                result["enum_definition"] = std::move(enum_list);
            }

            result["unique"] = column_definition.is_unique();
            result["auto"] = column_definition.is_auto();
            if (column_definition.is_hidden()) { result["hidden"] = column_definition.is_hidden(); }
            if (column_definition.is_readonly()) { result["readonly"] = column_definition.is_readonly(); }
            if (column_definition.is_internal()) { result["internal"] = column_definition.is_internal(); }
            result["default_value"] = column_definition.get_default_value();
            if (!column_definition.get_description().empty())
                result["description"] = column_definition.
                    get_description();

            return result;
        };


        auto custom_action_to_json = [](const mindnet::model::CustomAction& custom_action)
        {
            crow::json::wvalue result;

            result["action"] = custom_action.action;
            result["label"] = custom_action.label;
            result["crudl"] = custom_action.crudl;
            result["model_name"] = custom_action.model_name;

            if (!custom_action.params.empty())
            {
                crow::json::wvalue params_as_json;

                for (auto& entry : custom_action.params)
                {
                    params_as_json[entry.first] = entry.second; // [ "id", 1
                }

                result["params"] = std::move(params_as_json);
            }

            return result;
        };


        auto model_definition_to_json = [column_definition_to_json, service_ptr, custom_action_to_json](
            string& model_name,
            const std::set<string>& fields_set
        )
        {
            auto model_definition = service_ptr->get_model_definition(model_name);
            crow::json::wvalue res;
            if (
                model_definition->get_allowed_rest_operations().empty()
            )
            {
                return res;
            }

            auto fields_set_empty = fields_set.empty();
            if (fields_set_empty || fields_set.contains("model_name"))
            {
                res["model_name"] = model_definition->get_model_name();
            }
            if (fields_set_empty || fields_set.contains("group"))
            {
                res["group"] = model_definition->get_group();
                if (fields_set_empty || fields_set.contains("group_order_index"))
                {
                    res["group_order_index"] = model_definition->get_group_order_index();
                }
            }
            crow::json::wvalue::list crudl_list;
            for (auto e : model_definition->get_allowed_rest_operations())
            {
                crudl_list.push_back(crudl_to_string(e));
            }

            if (fields_set_empty || fields_set.contains("allowed_rest_operations"))
            {
                res["allowed_rest_operations"] = std::move(crudl_list);
            }
            if (fields_set_empty || fields_set.contains("virtual_table"))
            {
                res["virtual_table"] = model_definition->is_virtual_table();
            }
            if (fields_set_empty || fields_set.contains("no_table"))
            {
                res["no_table"] = model_definition->is_no_table();
            }

            if (fields_set_empty || fields_set.contains("title_column") && !model_definition->get_title_column().
                empty())
            {
                res["title_column"] = model_definition->get_title_column();
            }

            if (fields_set_empty || fields_set.contains("reader_can_write"))
            {
                res["reader_can_write"] = model_definition->is_reader_can_write();
            }
            if (fields_set_empty || fields_set.contains("read_cache_enabled"))
            {
                res["read_cache_enabled"] = model_definition->is_read_cache_enabled();
            }

            //
            if (fields_set_empty || fields_set.contains("columns"))
            {
                crow::json::wvalue::list column_list;
                for (auto e : model_definition->get_columns())
                {
                    auto column_as_json = column_definition_to_json(e);
                    //if (column_as_json.t() == crow::json::type::Null) {continue;}
                    column_list.push_back(column_as_json);
                }
                res["columns"] = std::move(column_list);
            }
            if (fields_set_empty || fields_set.contains("custom_actions"))
            {
                crow::json::wvalue::list custom_action_list;
                for (auto ca : model_definition->get_custom_actions())
                {
                    auto custom_action_as_json_ = custom_action_to_json(ca);
                    custom_action_list.push_back(custom_action_as_json_);
                }
                res["custom_actions"] = std::move(custom_action_list);
            }


            return res;
        };

        //CREATE
        CROW_ROUTE(crow_app, "/api/v1/model_definition").methods(crow::HTTPMethod::POST)
        ([service_ptr]
        {
            check_maintenance_mode()
            return crow::response(405, "Method not allowed for model_definition.");
        });


        //READ
        CROW_ROUTE(crow_app, "/api/v1/model_definition/<string>").methods(crow::HTTPMethod::GET)
        ([service_ptr, model_definition_to_json](const crow::request& req, string model_name)
        {
            check_maintenance_mode()

            if (!service_ptr->has_model(model_name))
            {
                return crow::response(404, "Model definition not found: " + model_name);
            }
            const string& fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::set<string> fields_set;
            util::Utils::split_string_by_commas(fields, fields_set);
            auto json = model_definition_to_json(model_name, fields_set);

            if (json.t() == crow::json::type::Null)
            {
                return crow::response(404, "Model definition not found: " + model_name);
            }

            return crow::response(200, json);
        });

        // UPDATE
        CROW_ROUTE(crow_app, "/api/v1/model_definition").methods(crow::HTTPMethod::PUT)
        ([service_ptr]
        {
            check_maintenance_mode()
            return crow::response(405, "Method not allowed for model_definition.");;
        });

        // DELETE
        CROW_ROUTE(crow_app, "/api/v1/model_definition").methods(crow::HTTPMethod::DELETE)
        ([service_ptr]
        {
            check_maintenance_mode()

            return crow::response(405, "Method not allowed for model_definition.");;
        });

        // LIST
        CROW_ROUTE(crow_app, "/api/v1/model_definition").methods(crow::HTTPMethod::GET)
        ([service_ptr, model_definition_to_json](const crow::request& req)
        {
            check_maintenance_mode()

            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::set<string> fields_set;
            mindnet::util::Utils::split_string_by_commas(fields, fields_set);
            crow::json::wvalue result;

            crow::json::wvalue::list model_definitions_as_json;
            for (auto& model_name : service_ptr->list_model_names())
            {
                //std::cout << model_name << std::endl;
                auto model_definition_as_json = model_definition_to_json(model_name, fields_set);
                if (model_definition_as_json.t() == crow::json::type::Null)
                {
                    continue;
                }
                model_definitions_as_json.push_back(model_definition_as_json);
            }
            result["items"] = std::move(model_definitions_as_json);
            return crow::response(200, result);
        });


        // LIST
        CROW_ROUTE(crow_app, "/api/v1/app").methods(crow::HTTPMethod::GET)
        ([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()


            crow::json::wvalue result;

            crow::json::wvalue::list apps_as_json;
            for (auto& plugin_name : service_ptr->get_plugin_registry()->get_plugin_names())
            {
                const auto& plugin = service_ptr->get_plugin_registry()->get_plugin(plugin_name);
                for (const auto& app : plugin->get_apps())
                {
                    apps_as_json.push_back(app);
                }
            }
            result["items"] = std::move(apps_as_json);
            return crow::response(200, result);
        });
    }
}
