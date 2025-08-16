//
// Created by robertvokac on 8/16/25.
//
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "crow.h"
#include "HttpUtils.h"

namespace miniwiki::http
{
    class HttpServer
    {

    public:
        HttpServer();
        void run(int port = 8080);
        template<typename T>
        void register_controller(T* controller) {
            controller->register_routes(crow_app);
        }

    private:
        crow::SimpleApp crow_app;

    };
}

#endif // HTTPSERVER_H
