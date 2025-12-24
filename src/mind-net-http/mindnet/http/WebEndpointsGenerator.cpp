/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/http/WebEndpointsGenerator.hpp"

#include <unordered_set>

#include "mindnet/http/ModelDefinitionEndpointsGenerator.hpp"

#include "mindnet/api/IService.hpp"

namespace mindnet::http
{
    namespace fs = std::filesystem;

    // ============================================================
    // Allowed root files
    // ============================================================
    static const std::unordered_set<std::string> generic_allowed_files = {
        "index.html",
        "styles.css",
        "common.css",
        "common.js",
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

    // ============================================================
    // Secure resolver
    // ============================================================
    static std::optional<fs::path>
    resolve_static_path(const fs::path& base, const fs::path& relative)
    {
        try
        {
            fs::path canonical_base = fs::canonical(base);
            fs::path canonical_req  = fs::weakly_canonical(canonical_base / relative);

            if (!canonical_req.native().starts_with(canonical_base.native()))
                return std::nullopt;

            if (!fs::exists(canonical_req) || fs::is_directory(canonical_req))
                return std::nullopt;

            return canonical_req;
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    // ============================================================
    // MIME types
    // ============================================================
    static std::string content_type_for(const fs::path& p)
    {
        static const std::unordered_map<std::string, std::string> mime_map = {
            {".html", "text/html"},
            {".css",  "text/css"},
            {".js",   "application/javascript"},
            {".png",  "image/png"},
            {".svg",  "image/svg+xml"},
            {".ico",  "image/x-icon"},
            {".json", "application/json"}
        };

        auto it = mime_map.find(p.extension().string());
        return it != mime_map.end()
            ? it->second
            : "application/octet-stream";
    }
    // ============================================================
    // ctor
    // ============================================================
    WebEndpointsGenerator::WebEndpointsGenerator(
        const std::string& directory_for_static_files_)
        : directory_for_static_files(directory_for_static_files_)
    {
    }

    // ============================================================
    // create endpoints
    // ============================================================
    void WebEndpointsGenerator::create_web_endpoints(
        const api::ServicePtr& service_ptr_,
        crow::SimpleApp& crow_app)
    {
        // ========================================================
        // /web/<path>
        // ========================================================
        CROW_ROUTE(crow_app, "/web/<path>")
        ([this, service_ptr_](const crow::request&, crow::response& res, const std::string& requested)
        {
            if (essential::g_configuration.access_mode
                == essential::AccessMode::MaintenanceMode)
            {
                res.code = 503;
                res.write("Maintenance Mode. Service Unavailable.");
                res.end();
                return;
            }

            fs::path rel = requested;
            fs::path root = directory_for_static_files;
            std::optional<fs::path> resolved;

            // ----------------------------------------------------
            // 1) ROOT: only whitelisted files + app_*.{html,css,js}
            // ----------------------------------------------------
            if (rel.begin() != rel.end() && ++rel.begin() == rel.end())
            {
                const std::string filename = rel.filename().string();

                bool allowed = generic_allowed_files.contains(filename);

                if (!allowed)
                {
                    auto& registry = service_ptr_->get_plugin_registry();
                    for (const auto& plugin_name : registry->get_plugin_names())
                    {
                        const auto& plugin = registry->get_plugin(plugin_name);
                        for (const auto& app : plugin->get_apps())
                        {
                            if (filename == "app_" + app + ".html" ||
                                filename == "app_" + app + ".css"  ||
                                filename == "app_" + app + ".js")
                            {
                                allowed = true;
                                break;
                            }
                        }
                        if (allowed) break;
                    }
                }

                if (!allowed)
                {
                    res.code = 403;
                    res.write("Access denied");
                    res.end();
                    return;
                }

                resolved = resolve_static_path(root, rel);
            }
            // ----------------------------------------------------
            // 2) library/** → only .js .css .html
            // ----------------------------------------------------
            else if (
                rel.has_parent_path() &&
                rel.parent_path() == fs::path("libraries")
            )
            {
                fs::path rel_path = rel;
                std::string filename = rel_path.filename().string();

                auto& registry = service_ptr_->get_plugin_registry();
                bool plugin_file_found = false;
                for (const auto& plugin_name : registry->get_plugin_names())
                {
                    auto plugin = registry->get_plugin(plugin_name);
                    const auto& library_files = plugin->get_library_files();
                    for (const auto& lf: library_files)
                    {
                        if (filename == lf)
                        {
                            plugin_file_found = true;
                            break;
                        }
                    }
                    if (plugin_file_found) break;
                }
                if (!plugin_file_found)
                {
                    res.code = 403;
                    res.write("Library file not allowed");
                    res.end();
                    return;
                }
                // auto ext = rel.extension().string();
                // if (ext != ".js" && ext != ".css" && ext != ".html")
                // {
                //     res.code = 403;
                //     res.write("Invalid library file type");
                //     res.end();
                //     return;
                // }

                resolved = resolve_static_path(
                    root / "libraries",
                    rel.lexically_relative("libraries"));
            }
            // ----------------------------------------------------
            // 3) plugin/** → anything inside plugin dir
            // ----------------------------------------------------
            else
            {
                auto it = rel.begin();
                std::string plugin_name = it->string();

                auto& registry = service_ptr_->get_plugin_registry();
                if (!registry->has_plugin_name(plugin_name))
                {
                    res.code = 403;
                    res.write("Unknown plugin");
                    res.end();
                    return;
                }

                resolved = resolve_static_path(
                    root / plugin_name,
                    rel.lexically_relative(plugin_name));
            }

            if (!resolved)
            {
                res.code = 403;
                res.write("Access denied");
                res.end();
                return;
            }

            // ----------------------------------------------------
            // cache
            // ----------------------------------------------------
            fs::path file_path = *resolved;
            std::string cache_key = file_path.string();
            auto last_mod = fs::last_write_time(file_path);

            auto it_cache = file_cache.find(cache_key);
            if (it_cache != file_cache.end()
                && it_cache->second.last_modified == last_mod)
            {
                res.set_header("Content-Type", content_type_for(file_path));
                res.write(it_cache->second.content);
                res.end();
                return;
            }

            // ----------------------------------------------------
            // read file
            // ----------------------------------------------------
            std::ifstream file(file_path, std::ios::binary);
            if (!file)
            {
                res.code = 500;
                res.write("Failed to open file");
                res.end();
                return;
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            file_cache[cache_key] = CachedFile{content, last_mod};

            static const std::string CONTENT_TYPE = "Content-Type";
            res.set_header(CONTENT_TYPE, content_type_for(file_path));
            res.write(content);
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