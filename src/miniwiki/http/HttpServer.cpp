#include "miniwiki/http/HttpServer.h"

namespace miniwiki::http {

    HttpServer::HttpServer() {
        //
    }

    void HttpServer::run(int port) {
        crow_app.port(port).multithreaded().run();
    }

}
