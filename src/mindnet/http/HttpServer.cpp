#include "mindnet/http/HttpServer.h"

namespace mindnet::http {

    HttpServer::HttpServer(std::shared_ptr<persistence::Persistence> db) : db_(db)
    {
        CROW_ROUTE(crow_app, "/")([](const crow::request&, crow::response& res){
    res.set_static_file_info("static/index.html");
    res.end();
});

    }

    void HttpServer::run(int port) {
        crow_app.port(port).multithreaded().run();
    }

}
