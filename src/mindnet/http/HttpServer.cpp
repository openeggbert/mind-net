#include "mindnet/http/HttpServer.h"

#include <unordered_set>
#include <utility>
#include <utility>

#include "mindnet/Global.h"

namespace mindnet::http
{
    HttpServer::HttpServer(std::shared_ptr<persistence::Persistence> db,
                           const std::string& directory_for_static_files)
        : db_(std::move(db))
    {
        CROW_ROUTE(crow_app, "/web/<string>")
        ([directory_for_static_files, this](const crow::request&, crow::response& res, const std::string& filename)
        {
            if (filename.find("..") != std::string::npos)
            {
                res.code = 403;
                res.write("Path traversal attempt blocked");
                res.end();
                return;
            }

            static const std::unordered_set<std::string> allowed_files = {
                "index.html", "styles.css", "scripts.js"
            };

            if (allowed_files.find(filename) == allowed_files.end())
            {
                res.code = 403;
                res.write("Access denied");
                res.end();
                return;
            }

            namespace fs = std::filesystem;
            {
                fs::path base_path = fs::canonical(directory_for_static_files);
                fs::path requested_path = fs::weakly_canonical(base_path / filename);


                if (requested_path.string().find(base_path.string()) != 0)
                {
                    res.code = 403;
                    res.write("Access denied");
                    res.end();
                    return;
                }
            }
            std::string full_path = directory_for_static_files + "/" + filename;
            fs::path file_path(full_path);

            if (!fs::exists(file_path))
            {
                res.code = 404;
                res.write("File not found");
                res.end();
                return;
            }

            auto last_mod = fs::last_write_time(file_path);
            auto it = file_cache.find(filename);

            if (it != file_cache.end() && it->second.last_modified == last_mod)
            {
                // Serve from cache
                if (filename.ends_with(".css"))
                {
                    res.set_header("Content-Type", "text/css");
                }
                else if (filename.ends_with(".js"))
                {
                    res.set_header("Content-Type", "application/javascript");
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

            file_cache[filename] = CachedFile{file_content, last_mod};

            if (filename.ends_with(".css"))
            {
                res.set_header("Content-Type", "text/css");
            }
            else if (filename.ends_with(".js"))
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

    void HttpServer::run(int port)
    {
        crow_app.port(port).multithreaded().run();
    }
}
