#include "mindnet/http/HttpServer.h"

#include <unordered_set>
#include <utility>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "mindnet/Global.h"
#include "jwt-cpp/jwt.h"
#include "mindnet/Configuration.h"
#include "mindnet/Service.h"
#include "mindnet/http/LoginToken.h"
#include "mindnet/http/UserCredentials.h"
#include "mindnet/plugins/core/models/User.h"

namespace mindnet::http
{
    HttpServer::HttpServer(ServicePtr& service_ptr,
                           const std::string& directory_for_static_files_)
        : service_ptr_(service_ptr),
          directory_for_static_files(directory_for_static_files_)
    {
        create_web_endpoints();

        create_model_definition_endpoints(service_ptr);
        create_authentication_endpoints(service_ptr);
    }

    void HttpServer::run(const string& host, int port, int frontend_port)
    {
        namespace fs = std::filesystem;

        ////
#ifndef jstxt
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
            js_file << "//" << file.filename().string() << ":" << std::endl;
            std::ifstream input(file, std::ios::binary);
            js_file << input.rdbuf() << std::endl;
        }
        ///
#endif

        //
        fs::path conf_js_path = fs::path(directory_for_static_files) / "conf.js";
        if (fs::exists(conf_js_path))
        {
            fs::remove(conf_js_path);
        }
        std::ofstream conf_js(conf_js_path);
        conf_js << "export const HOST = \"" << host << "\";" << std::endl;
        conf_js << "export const PORT = " << frontend_port << ";" << std::endl;
        conf_js.close();

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
                "conf.js",
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

    const std::pmr::set<string> forbidden_model_names = {
        "comment",
        "discussion",
        "message",
        "review",
        "sm2_state",
        "suggestion",
        "suggestion_review",
    };

    void HttpServer::create_model_definition_endpoints(const ServicePtr& service_ptr)
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
                //|| forbidden_model_names.contains(model_name)
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
        CROW_ROUTE(crow_app, "/api/model_definition").methods(crow::HTTPMethod::POST)
            ([] { return crow::response(405, "Method not allowed for model_definition.");; });


        //READ
        CROW_ROUTE(crow_app, "/api/model_definition/<string>").methods(crow::HTTPMethod::GET)
        ([service_ptr, model_definition_to_json, split_string_by_commas](const crow::request& req, string model_name)
        {
            if (!service_ptr->has_model(model_name))
            {
                return crow::response(404, "Model definition not found: " + model_name);
            }
            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::set<string> fields_set;
            split_string_by_commas(fields, fields_set);
            auto json = model_definition_to_json(model_name, fields_set);

            if (json.t() == crow::json::type::Null)
            {
                return crow::response(404, "Model definition not found: " + model_name);
            }

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
        ([service_ptr, model_definition_to_json, split_string_by_commas](const crow::request& req)
        {
            string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

            std::set<string> fields_set;
            split_string_by_commas(fields, fields_set);
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

            //return model_definition_to_json(model_name);
        });
    }

    inline std::string hash_password(const std::string& pass)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(pass.c_str()), pass.size(), hash);

        std::ostringstream os;
        for (unsigned char i : hash)
            os << std::hex << std::setw(2) << std::setfill('0') << (int)i;

        return os.str();
    }

#include <random>
#include <string>

    //openssl rand -base64 32

    std::string generate_secret_key(size_t length = 32)
    {
        static const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "!@#$%^&*()-_=+[]{}<>?/|";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

        std::string key;
        key.reserve(length);
        for (size_t i = 0; i < length; i++)
        {
            key.push_back(charset[dist(gen)]);
        }
        return key;
    }

    string get_jwt_secret()
    {
        return g_configuration.jwt_secret;
        // const char* env_secret = std::getenv("JWT_SECRET");
        // std::string secret = env_secret ? env_secret : "";
        // if (secret.empty())
        // {
        //     throw std::runtime_error("JWT_SECRET environment variable is not set.");
        // }
        // return secret;
    }

    void HttpServer::create_authentication_endpoints(const ServicePtr& service_ptr)
    {
        CROW_ROUTE(crow_app, "/login").methods("POST"_method)([service_ptr](const crow::request& req)
        {
            UserCredentials credentials = req;
            if (!credentials.error.empty())
            {
                return crow::response(400, credentials.error);
            }
            string error;
            QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, credentials.username);
            LoginToken login_token{req};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (users.first.empty()) { return crow::response(401, "User does not exist."); }
            plugins::core::models::User user;
            user.from_values(users.first[0]);

            string expected_password_hash = user.password_hash;
            string returned_password_hash = hash_password(credentials.password);
            bool verified = expected_password_hash == returned_password_hash;
            if (verified)
            {
                using namespace std::chrono_literals;
                auto token = jwt::create()
                             .set_issuer("crow-app")
                             .set_type("JWT")
                             .set_payload_claim("username", jwt::claim(credentials.username))
                             .set_payload_claim("user_id", jwt::claim(std::to_string(user.get_id())))
                             .set_expires_at(std::chrono::system_clock::now() + 7 * 24h)
                             .sign(jwt::algorithm::hs256{get_jwt_secret()});

                crow::json::wvalue response;
                response["token"] = token;
                return crow::response{response};
            }
            else
            {
                return crow::response(401, "Username or password is not correct.");
            }
        });

        CROW_ROUTE(crow_app, "/register").methods("POST"_method)([=](const crow::request& req)
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password"))
                return crow::response{400};

            std::string username = body["username"].s();
            std::string password = body["password"].s();
            std::string display_name = body["display_name"].s();
            std::string profile_text = body["profile_text"].s();
            std::string email = body["email"].s();

            //
            string error;
            QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, username);
            query_params.fields = {plugins::core::columns::UserColumns::USERNAME};
            LoginToken login_token{req};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (!error.empty()) { return crow::response(500, "Checking, if user already exists, failed. " + error); }
            if (!users.first.empty()) { return crow::response(409, "User already exists."); }
            //

            std::string hashed = hash_password(password);
            plugins::core::models::User user;
            user.username = username;
            user.password_hash = hashed;
            user.display_name = display_name;
            user.role = plugins::core::enums::UserRole::READER;
            user.profile_text = profile_text;
            user.last_login = 0;
            user.email = email;
            user.status = plugins::core::enums::UserStatus::PENDING;

            error.clear();
            auto fields_ = user.to_values();
            service_ptr.get()->create(plugins::core::models::USER_DEFINITION, login_token, fields_);
            if (!error.empty())
            {
                return crow::response{400, "Registration failed. " + error};
            }

            return crow::response{201, "Registration successful"};
        });

        CROW_ROUTE(crow_app, "/protected")([](const crow::request& req)
        {
            LoginToken login_token{req};
            return crow::response(login_token.status, login_token.msg);
        });
    }
}
