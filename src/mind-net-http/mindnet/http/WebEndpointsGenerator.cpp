//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/WebEndpointsGenerator.h"

#include <unordered_set>

#include "mindnet/http/ModelDefinitionEndpointsGenerator.h"

#include "mindnet/api/IService.h"

namespace mindnet::http
{
    WebEndpointsGenerator::WebEndpointsGenerator(const std::string& directory_for_static_files_)
        :directory_for_static_files(directory_for_static_files_)
    {
    }

    void WebEndpointsGenerator::create_web_endpoints(
        const api::ServicePtr& service_ptr_,
        crow::SimpleApp& crow_app
    )
    {
        CROW_ROUTE(crow_app, "/web/<string>")
        ([this, service_ptr_](const crow::request& req, crow::response& res, const std::string& file_name)
        {
            if (essential::g_configuration.access_mode == essential::AccessMode::MaintenanceMode)
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
                "common.css",
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
                "auth.js",
                "auth-ui.js"
            };
            static std::unordered_set<std::string> plugin_allowed_files;


            auto& plugin_registry = service_ptr_->get_plugin_registry();
            for (const auto& plugin_name : plugin_registry->get_plugin_names())
            {
                const auto& plugin = plugin_registry->get_plugin(plugin_name);
                for (auto& app_name : plugin->get_apps())
                {
                    string path_prefix = "app_" + app_name;
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

            // for (auto& e : common_allowed_files)std::cout << "Allowed: " << e << std::endl;
            // for (auto& e : plugin_allowed_files)std::cout << "Allowed: " << e << std::endl;

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
                res.write("File not found: " + file_path.string());
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
            if (essential::g_configuration.access_mode == essential::AccessMode::MaintenanceMode)
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
}
