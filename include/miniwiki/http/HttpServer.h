//
// Created by robertvokac on 8/16/25.
//
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "crow.h"
#include "HttpUtils.h"
#include "miniwiki/persistence/Persistence.h"

namespace miniwiki::http
{
    class HttpServer
    {

    public:
        HttpServer(std::shared_ptr<persistence::Persistence> db);
        void run(int port = 8080);
        template<typename T>
        void register_controller(T* controller) {
            controller->register_routes(crow_app, db_);
        }

    private:
        crow::SimpleApp crow_app;
        std::shared_ptr<persistence::Persistence> db_;

    };
}

#endif // HTTPSERVER_H
