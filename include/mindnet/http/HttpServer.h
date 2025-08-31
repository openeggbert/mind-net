//
// Created by robertvokac on 8/16/25.
//
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "crow.h"
#include "HttpUtils.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::http
{
    struct CachedFile
    {
        std::string content;
        std::filesystem::file_time_type last_modified;
    };

    class HttpServer
    {
    public:
        HttpServer(std::shared_ptr<persistence::Persistence> db,
                   const std::string& directory_for_static_files = std::string("static"));
        void run(int port = 8080);

        template <typename T>
        void register_controller(T* controller, models::misc::ModelDefinition& definition)
        {
            controller->register_routes(crow_app, db_, definition);
        }

        crow::SimpleApp& get_crow_app();

    private:
        crow::SimpleApp crow_app;
        std::shared_ptr<persistence::Persistence> db_;
        std::unordered_map<std::string, CachedFile> file_cache;
        //
        void create_web_endpoints(const std::string& directory_for_static_files);
        void create_model_definition_endpoints(const std::shared_ptr<persistence::Persistence>& d_b_);
    };
}

#endif // HTTPSERVER_H
