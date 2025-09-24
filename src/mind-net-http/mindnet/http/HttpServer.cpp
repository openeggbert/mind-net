#include "mindnet/http/HttpServer.h"

#include <unordered_set>
#include <utility>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "SuperAdminEndpointsGenerator.h"
#include "mindnet/essential/Global.h"
#include "jwt-cpp/jwt.h"
#include "mindnet/essential/Configuration.h"
#include "mindnet/api/Service.h"
#include "mindnet/essential/Version.h"
#include "mindnet/api/LoginToken.h"
#include "mindnet/http/UserCredentials.h"
#include "mindnet/plugins/core/CorePersistenceMethods.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/util/Utils.h"
#define check_maintenance_mode()\
if (g_configuration.access_mode == essential::AccessMode::MaintenanceMode)\
return crow::response(503, "Maintenance Mode. Service Unavailable.");

namespace mindnet::http
{
    using mindnet::essential::g_configuration;

    namespace Labels
    {
        static const std::string DAY = " day ";
        static const std::string DAYS = " days ";
        static const std::string HOUR = " hour ";
        static const std::string HOURS = " hours ";
        static const std::string MINUTE = " minute ";
        static const std::string MINUTES = " minutes ";
        static const std::string SECOND = " second";
        static const std::string SECONDS = " seconds";
    }

    HttpServer::HttpServer(api::ServicePtr& service_ptr,
                           std::string directory_for_static_files_)
        : service_ptr_(service_ptr),
          directory_for_static_files(std::move(directory_for_static_files_))
    {
        create_web_endpoints();

        create_model_definition_endpoints(service_ptr);
        create_authentication_endpoints(service_ptr);
        create_superadmin_endpoints(service_ptr);
        create_info_endpoint(service_ptr);
        create_health_endpoint(service_ptr);
    }

    void HttpServer::request_shutdown()
    {
        std::thread([this]
        {
            std::this_thread::sleep_for(std::chrono::seconds{5L});
            crow_app.stop();
            std::exit(0); // clean exit, systemd won't restart unless Restart=always
        }).detach();
    }

    void HttpServer::request_restart()
    {
        std::thread([this]
        {
            std::this_thread::sleep_for(std::chrono::seconds{5L});
            crow_app.stop();
            std::_Exit(42); // restart, systemd will handle
        }).detach();
    }

    void HttpServer::run(const string& host, int port, int frontend_port)
    {
        namespace fs = std::filesystem;


        //#define generate_js_files_to_js_txt
#ifdef generate_js_files_to_js_txt
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

    // crow::SimpleApp& HttpServer::get_crow_app()
    // {
    //     return crow_app;
    // }


    void HttpServer::create_web_endpoints()
    {
        CROW_ROUTE(crow_app, "/web/<string>")
        ([this](const crow::request& req, crow::response& res, const std::string& file_name)
        {
            if (g_configuration.access_mode == essential::AccessMode::MaintenanceMode)
            {
                res.code = 503;
                res.write("Maintenance Mode. Service Unavailable.");
                res.end();
                return;
            }

            if (file_name.find("..") != std::string::npos)
            {
                res.code = 403;
                res.write("Path traversal attempt blocked");
                res.end();
                return;
            }

            static const std::unordered_set<std::string> common_allowed_files = {
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
            static std::unordered_set<std::string> plugin_allowed_files;


            auto& plugin_registry = service_ptr_->get_plugin_registry();
            for (const auto& plugin_name : plugin_registry->get_plugin_names())
            {
                const auto& plugin = plugin_registry->get_plugin(plugin_name);
                for (auto& app_name : plugin->get_apps())
                {
                    string path_prefix = app_name += plugin_name + "/";
                    plugin_allowed_files.insert(path_prefix + ".html");
                    plugin_allowed_files.insert(path_prefix + ".css");
                    plugin_allowed_files.insert(path_prefix + ".js");
                }
            }

            if (common_allowed_files.find(file_name) == common_allowed_files.end()
                &&
                plugin_allowed_files.find(file_name) == plugin_allowed_files.end()
            )
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
            if (g_configuration.access_mode == essential::AccessMode::MaintenanceMode)
            {
                res.code = 503;
                res.write("Maintenance Mode. Service Unavailable.");
                res.end();
                return;
            }

            res.redirect("/web/index.html");
            res.end();
        });
    }

    void HttpServer::create_model_definition_endpoints(const api::ServicePtr& service_ptr)
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

            if (fields_set_empty || fields_set.contains("title_column") && !model_definition->get_title_column().
                empty())
            {
                res["title_column"] = model_definition->get_title_column();
            }

            if (fields_set_empty || fields_set.contains("reader_can_write"))
            {
                res["reader_can_write"] = model_definition->is_reader_can_write();
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
        ([]
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
        ([]
        {
            check_maintenance_mode()
            return crow::response(405, "Method not allowed for model_definition.");;
        });

        // DELETE
        CROW_ROUTE(crow_app, "/api/v1/model_definition").methods(crow::HTTPMethod::DELETE)
        ([]
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
            util::Utils::split_string_by_commas(fields, fields_set);
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
    }

    void HttpServer::create_info_endpoint(const api::ServicePtr& service_ptr)
    {
        //READ
        CROW_ROUTE(crow_app, "/info").methods(crow::HTTPMethod::GET)
        ([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()

            nlohmann::ordered_json result;

            result["name"] = g_configuration.name;
            result["description"] = g_configuration.description;
            result["version"] = STRINGIFY(MIND_NET_VERSION);
            result["build_time"] = MIND_NET_BUILD_TIME;
            result["environment"] = environment_to_string(g_configuration.environment);
            //
            result["host"] = g_configuration.host;
            result["port"] = g_configuration.port;
            result["frontend_port"] = g_configuration.frontend_port;
            //
            result["access_mode"] = access_mode_to_string(g_configuration.access_mode);
            result["registration_mode"] = registration_mode_to_string(g_configuration.registration_mode);
            result["default_user_role"] = user_role_to_string(g_configuration.default_user_role);

            return crow::response(200, result.dump(2));
        });
    }

    void HttpServer::create_health_endpoint(const api::ServicePtr& service_ptr)
    {
        //READ
        CROW_ROUTE(crow_app, "/health").methods(crow::HTTPMethod::GET)
        ([service_ptr](const crow::request& req)
        {
            auto print_duration = [](ll start_time, ll end_time)
            {
                static const int SECONDS_PER_DAY = 24 * 60 * 60;
                static const int SECONDS_PER_HOUR = 60 * 60;
                static const int SECONDS_PER_MINUTE = 60;

                //#define test_health_endpoint
#ifdef test_health_endpoint
                std::random_device rd;
                std::mt19937 gen(rd());
                enum time_type
                {
                    seconds_ = 0,
                    minutes_ = 1,
                    hours_ = 2,
                    days_ = 3,
                    weeks_ = 4,
                    months_ = 5,
                    years_ = 6
                };

                std::uniform_int_distribution<> distrib0(0, 7);
                time_type random_time_type = static_cast<time_type>(distrib0(gen));
                int max = 0;
                switch (random_time_type)
                {
                case seconds_: max = 1 * 10;
                    break;
                case minutes_: max = 60 * 10;
                    break;
                case hours_: max = 3600 * 10;
                    break;
                case days_: max = 86400 * 10;
                    break;
                case weeks_: max = 86400 * 7 * 10;
                    break;
                case months_: max = 86400 * 30 * 10;
                    break;
                case years_: max = 86400 * 365 * 10;
                    break;
                default: max = 1000000;
                }
                std::uniform_int_distribution<> distrib(0, max);


                ll elapsed_seconds = end_time - start_time + distrib(gen);
#else

                ll elapsed_seconds = end_time - start_time;
#endif
                short days = 0;
                short hours = 0;
                short minutes = 0;
                short seconds = 0;
                days = elapsed_seconds / SECONDS_PER_DAY;
                elapsed_seconds -= days * SECONDS_PER_DAY;
                hours = elapsed_seconds / SECONDS_PER_HOUR;
                elapsed_seconds -= hours * SECONDS_PER_HOUR;
                minutes = elapsed_seconds / SECONDS_PER_MINUTE;
                elapsed_seconds -= minutes * SECONDS_PER_MINUTE;
                seconds = elapsed_seconds;
                std::ostringstream oss;
                if (days > 0)
                {
                    oss << days << (days == 1 ? Labels::DAY : Labels::DAYS);
                }
                if (hours > 0)
                {
                    oss << hours << (hours == 1 ? Labels::HOUR : Labels::HOURS);
                }
                if (minutes > 0)
                {
                    oss << minutes << (minutes == 1 ? Labels::MINUTE : Labels::MINUTES);
                }
                oss << seconds << (seconds == 1 ? Labels::SECOND : Labels::SECONDS);

                return oss.str();
            };

            nlohmann::ordered_json result;

            auto now = util::Utils::currentUnixTimestamp();
            result["status"] = g_configuration.access_mode == essential::AccessMode::MaintenanceMode
                                   ? "MAINTENANCE"
                                   : "UP";
            result["uptime"] = print_duration(essential::start_time, now);
            result["timestamp"] = util::Utils::unixToFormattedString(now);
            result["started_at"] = util::Utils::unixToFormattedString(essential::start_time);

            return crow::response(200, result.dump(2));
        });
    }

    void HttpServer::create_superadmin_endpoints(const api::ServicePtr& service_ptr)
    {
        SuperAdminEndpointsGenerator gen;
        gen.create_superadmin_endpoints(
            service_ptr,
            crow_app,
            [this]{ request_restart(); },
            [this]{ request_shutdown(); }
            );
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
    }

    void HttpServer::create_authentication_endpoints(const api::ServicePtr& service_ptr)
    {
        CROW_ROUTE(crow_app, "/api/v1/auth/login").methods("POST"_method)([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()

            UserCredentials credentials = req;
            if (!credentials.error.empty())
            {
                return crow::response(400, credentials.error);
            }
            string error;
            orm::QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, credentials.username);
            api::LoginToken login_token{req};
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

        CROW_ROUTE(crow_app, "/api/v1/auth/register").methods("POST"_method)([=](const crow::request& req)
        {
            check_maintenance_mode()

            if (g_configuration.registration_mode == essential::RegistrationMode::AdminAddsUsers)
            {
                return crow::response{405, "Endpoint /register is disabled. Only admin can add new users."};
            }
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password"))
                return crow::response{400};

            std::string username = body["username"].s();
            std::string password = body["password"].s();
            std::string display_name = body["display_name"].s();
            std::string profile_text = body["profile_text"].s();
            std::string email = body["email"].s();

            //

            orm::QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, username);
            query_params.fields = {plugins::core::columns::UserColumns::USERNAME};
            api::LoginToken login_token{req};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (users.second.ko())
            {
                return crow::response(500, "Checking, if user already exists, failed. " + users.second.error);
            }
            if (!users.first.empty()) { return crow::response(409, "User already exists."); }
            //

            std::string hashed = hash_password(password);
            plugins::core::models::User user;
            user.username = username;
            user.password_hash = hashed;
            user.display_name = display_name;
            user.role = mindnet::essential::UserRole::Reader;
            user.profile_text = profile_text;
            user.last_login = 0;
            user.email = email;
            user.status = essential::UserStatus::Active;

            auto fields_ = user.to_values();
            auto create_result = service_ptr.get()->
                                             create(plugins::core::models::USER_DEFINITION, login_token, fields_);
            if (create_result.second.ko())
            {
                return crow::response{400, "Registration failed. " + create_result.second.error};
            }

            return crow::response{201, "Registration successful"};
        });

        CROW_ROUTE(crow_app, "/api/v1/protected")([](const crow::request& req)
        {
            check_maintenance_mode()

            api::LoginToken login_token{req};
            return crow::response(login_token.status, login_token.msg);
        });
    }
}
