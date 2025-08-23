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

        // Serve /web and / directly as index.html
        CROW_ROUTE(crow_app, "/web")
        ([directory_for_static_files, this](const crow::request&, crow::response& res) {
            std::string full_path = directory_for_static_files + "/index.html";
            namespace fs = std::filesystem;
            fs::path file_path(full_path);

            if (!fs::exists(file_path)) {
                res.code = 404;
                res.write("Not found");
                res.end();
                return;
            }

            auto last_mod = fs::last_write_time(file_path);
            auto it = file_cache.find("index.html");
            if (it != file_cache.end() && it->second.last_modified == last_mod) {
                res.set_header("Content-Type", "text/html");
                res.write(it->second.content);
            } else {
                std::ifstream file(full_path);
                if (!file.is_open()) {
                    res.code = 500;
                    res.write("Failed to open file");
                    res.end();
                    return;
                }

                std::ostringstream content;
                content << file.rdbuf();
                file_cache["index.html"] = CachedFile{content.str(), last_mod};

                res.set_header("Content-Type", "text/html");
                res.write(content.str());
            }

            res.end();
        });

        CROW_ROUTE(crow_app, "/")
        ([](const crow::request&, crow::response& res) {
            res.redirect("/web");
            res.end();
        });

    }

    void HttpServer::run(int port)
    {
        crow_app.port(port).multithreaded().run();
    }
}
