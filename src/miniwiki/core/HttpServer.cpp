#include "miniwiki/core/HttpServer.h"

namespace miniwiki::core {

    HttpServer::HttpServer() {
        //
    }

    void HttpServer::run() {
        app_.port(8080).multithreaded().run();
    }

}
