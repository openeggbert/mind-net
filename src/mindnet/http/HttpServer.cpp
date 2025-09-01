#include "mindnet/http/HttpServer.h"

#include <unordered_set>
#include <utility>
#include <utility>
#include <fstream>
#include <filesystem>

#include "mindnet/Global.h"

namespace mindnet::http
{
    HttpServer::HttpServer(std::shared_ptr<persistence::Persistence> db,
                           const std::string& directory_for_static_files_)
        : db_(std::move(db)),
          directory_for_static_files(directory_for_static_files_)
    {
        create_web_endpoints();

        create_model_definition_endpoints(db_);
    }

    void HttpServer::run(int port)
    {
        namespace fs = std::filesystem;

        ////
        std::ofstream js_file(fs::path(directory_for_static_files) / "js.txt");
        std::vector<fs::path> js_files;
        for (const auto& entry : fs::directory_iterator(directory_for_static_files))
        {
            if (entry.path().extension() == ".js")
            {
                js_files.push_back(entry.path());
            }
        }
        std::sort(js_files.begin(), js_files.end());

        for (const auto& file : js_files)
        {
            js_file << file.filename().string() << ":" << std::endl;
            std::ifstream input(file, std::ios::binary);
            js_file << input.rdbuf() << std::endl;
        }
        //
        fs::path port_js_path = fs::path(directory_for_static_files) / "port.js";
        if (fs::exists(port_js_path))
        {
            fs::remove(port_js_path);
        }
        std::ofstream port_js(port_js_path);
        port_js << "export const PORT = " << port << ";" << std::endl;
        port_js.close();



        crow_app.port(port).multithreaded().run();
    }

    crow::SimpleApp& HttpServer::get_crow_app()
    {
        return crow_app;
    }


    void HttpServer::create_web_endpoints()
    {
        CROW_ROUTE(crow_app, "/web/<string>")
        ([this](const crow::request& req, crow::response& res, const std::string& file_name)
        {
            if (file_name.find("..") != std::string::npos)
            {
                res.code = 403;
                res.write("Path traversal attempt blocked");
                res.end();
                return;
            }

            static const std::unordered_set<std::string> allowed_files = {
                "index.html",
                "styles.css",
                "scripts.js",
                "port.js",
                "favicon.png",
                "api.js",
                "schemas.js",
                "state.js",
                "dom.js",
                "crud.js",
                "explore.js",
                "navigation.js",
                "actions.js",
                "init.js",


            };

            if (allowed_files.find(file_name) == allowed_files.end())
            {
                res.code = 403;
                res.write("Access denied");
                res.end();
                return;
            }

            namespace fs = std::filesystem;
            {
                fs::path base_path = fs::canonical(directory_for_static_files);
                fs::path requested_path = fs::weakly_canonical(base_path / file_name);


                if (requested_path.string().find(base_path.string()) != 0)
                {
                    res.code = 403;
                    res.write("Access denied");
                    res.end();
                    return;
                }
            }
            std::string full_path = directory_for_static_files + "/" + file_name;
            fs::path file_path(full_path);

            if (!fs::exists(file_path))
            {
                res.code = 404;
                res.write("File not found");
                res.end();
                return;
            }

            auto last_mod = fs::last_write_time(file_path);
            auto it = file_cache.find(file_name);

            if (it != file_cache.end() && it->second.last_modified == last_mod)
            {
                // Serve from cache
                if (file_name.ends_with(".css"))
                {
                    res.set_header("Content-Type", "text/css");
                }
                else if (file_name.ends_with(".js"))
                {
                    res.set_header("Content-Type", "application/javascript");
                }
                else if (file_name.ends_with(".png"))
                {
                    res.set_header("Content-Type", "image/png");
                }

                else
                {
                    res.set_header("Content-Type", "text/html");
                }

                res.write(it->second.content);
                res.end();
                return;
            }

            // Read from disk and update cache
            std::ifstream file(full_path, std::ios::binary);
            if (!file.is_open())
            {
                res.code = 500;
                res.write("Failed to open file");
                res.end();
                return;
            }

            std::ostringstream content;
            content << file.rdbuf();
            std::string file_content = content.str();

            file_cache[file_name] = CachedFile{file_content, last_mod};

            if (file_name.ends_with(".css"))
            {
                res.set_header("Content-Type", "text/css");
            }
            else if (file_name.ends_with(".js"))
            {
                res.set_header("Content-Type", "application/javascript");
            }
            else
            {
                res.set_header("Content-Type", "text/html");
            }

            res.write(file_content);
            res.end();
        });


        CROW_ROUTE(crow_app, "/")
        ([](const crow::request&, crow::response& res)
        {
            res.redirect("/web");
            res.end();
        });

        CROW_ROUTE(crow_app, "/web")
        ([](const crow::request&, crow::response& res)
        {
            res.redirect("/web/index.html");
            res.end();
        });
    }

    void HttpServer::create_model_definition_endpoints(const std::shared_ptr<persistence::Persistence>& d_b_)
    {
        //todo: remove this duplicity
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

        auto column_definition_to_json = [](mindnet::models::misc::ColumnDefinition& column_definition)
        {
            crow::json::wvalue result;

            result["column_name"] = column_definition.get_column_name();
            result["column_type"] = mindnet::enums::column_type_to_string(column_definition.get_column_type());
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
            result["default_value"] = column_definition.get_default_value();
            if (!column_definition.get_description().empty())
                result["description"] = column_definition.
                    get_description();

            return result;
        };


        auto custom_action_to_json = [](mindnet::models::misc::CustomAction& custom_action)
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


        auto model_definition_to_json = [column_definition_to_json, d_b_, custom_action_to_json](
            string& model_name,
            const std::set<string>& fields_set
        )
        {
            auto model_definition = d_b_->get_model_definition(model_name);
            crow::json::wvalue res;
            auto fields_set_empty = fields_set.empty();
            if (fields_set_empty || fields_set.contains("model_name"))
            {
                res["model_name"] = model_definition->get_model_name();
            }
            crow::json::wvalue::list crudl_list;
            for (auto e : model_definition->get_allowed_rest_operations())
            {
                crudl_list.push_back(mindnet::enums::crudl_to_string(e));
            }

            if (fields_set_empty || fields_set.contains("allowed_rest_operations"))
            {
                res["allowed_rest_operations"] = std::move(crudl_list);
            }
            if (fields_set_empty || fields_set.contains("virtual_table"))
            {
                res["virtual_table"] = model_definition->is_virtual_table();
            }
            //
            if (fields_set_empty || fields_set.contains("columns"))
            {
                crow::json::wvalue::list column_list;
                for (auto e : model_definition->get_columns())
                {
                    auto column_as_json = column_definition_to_json(e);
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
        //READ
        CROW_ROUTE(crow_app, "/api/model_definition/<string>").methods(crow::HTTPMethod::GET)
        ([d_b_, model_definition_to_json, split_string_by_commas](const crow::request& req, string model_name)
        {
            if (!d_b_->has_repository(model_name))
            {
                return crow::response(404, "Model definition not found: " + model_name);
            }
            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::set<string> fields_set;
            split_string_by_commas(fields, fields_set);
            auto json = model_definition_to_json(model_name, fields_set);

            return crow::response(200, json);
        });

        // UPDATE
        CROW_ROUTE(crow_app, "/api/model_definition").methods(crow::HTTPMethod::PUT)
            ([] { return crow::response(405, "Method not allowed for model_definition.");; });

        // DELETE
        CROW_ROUTE(crow_app, "/api/model_definition").methods(crow::HTTPMethod::DELETE)
            ([] { return crow::response(405, "Method not allowed for model_definition.");; });

        // LIST
        CROW_ROUTE(crow_app, "/api/model_definition").methods(crow::HTTPMethod::GET)
        ([d_b_, model_definition_to_json, split_string_by_commas](const crow::request& req)
        {
            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::set<string> fields_set;
            split_string_by_commas(fields, fields_set);
            crow::json::wvalue result;

            crow::json::wvalue::list model_definitions_as_json;
            for (auto& model_name : d_b_->list_repositories())
            {
                //std::cout << model_name << std::endl;
                auto model_definition_as_json = model_definition_to_json(model_name, fields_set);
                model_definitions_as_json.push_back(model_definition_as_json);
            }
            result["items"] = std::move(model_definitions_as_json);
            return crow::response(200, result);

            //return model_definition_to_json(model_name);
        });
    }
}
