#include "mindnet/http/HttpServer.h"

namespace mindnet::http {

    HttpServer::HttpServer(std::shared_ptr<persistence::Persistence> db) : db_(db)
    {
    }

    void HttpServer::run(int port) {
        crow_app.port(port).multithreaded().run();
    }

}
