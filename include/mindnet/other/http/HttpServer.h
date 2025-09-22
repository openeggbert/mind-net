//
// Created by robertvokac on 8/16/25.
//
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "crow.h"
#include "HttpUtils.h"
#include "mindnet/other/api/IService.h"

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
        HttpServer(api::ServicePtr& service_ptr,
                   const std::string& directory_for_static_files = std::string("static"));
        void run(const string& host = "http://localhost", int port = 8080, int frontend_port = 8080);

        template <typename T>
        void create_model_endpoint(T* controller, model::ModelDefinition& definition)
        {
            controller->create_model_endpoint(crow_app, service_ptr_, definition);
        }

        crow::SimpleApp& get_crow_app();

    private:
        crow::SimpleApp crow_app;
        api::ServicePtr service_ptr_;
        std::unordered_map<std::string, CachedFile> file_cache;
        std::string directory_for_static_files;
        //
        void create_web_endpoints();
        void create_model_definition_endpoints(const api::ServicePtr& service_ptr);
        void create_info_endpoint(const api::ServicePtr& service_ptr);
        void create_health_endpoint(const api::ServicePtr& service_ptr);
        void create_authentication_endpoints(const api::ServicePtr& service_ptr);
    };
}

#endif // HTTPSERVER_H
