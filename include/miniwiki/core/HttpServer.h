//
// Created by robertvokac on 8/16/25.
//
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "crow.h"

namespace miniwiki::core
{
    class HttpServer
    {
    private:
        crow::SimpleApp app_;
    public:
        HttpServer();
        void run();
    };
}

#endif // HTTPSERVER_H
