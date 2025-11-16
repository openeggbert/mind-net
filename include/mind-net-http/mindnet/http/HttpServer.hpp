//
// Created by robertvokac on 8/16/25.
//
#pragma once


#include "crow.h"
#include "HttpUtils.hpp"
#include "mindnet/api/IService.hpp"

namespace mindnet::http
{
    class WebEndpointsGenerator;

    class HttpServer
    {
    public:
        HttpServer(api::ServicePtr& service_ptr,
                   std::string directory_for_static_files = std::string("static"));
        void request_shutdown();
        void request_restart();
        void run(const string& host = "http://localhost", int port = 8080, int frontend_port = 8080);

        void create_info_health_endpoints(const api::ServicePtr& shared);

        template <typename T>
        void create_model_endpoint(T* controller, model::ModelDefinition& definition)
        {
            controller->create_model_endpoint(crow_app, service_ptr_, definition);
        }

        // crow::SimpleApp& get_crow_app();

    private:
        crow::SimpleApp crow_app;
        api::ServicePtr service_ptr_;
        std::string directory_for_static_files;
        std::shared_ptr<WebEndpointsGenerator> web_endpoints_generator_ = nullptr;


        //
        void create_web_endpoints(const api::ServicePtr& service_ptr);
        void create_model_definition_endpoints(const api::ServicePtr& service_ptr);
        void create_info_endpoint(const api::ServicePtr& service_ptr);
        void create_health_endpoint(const api::ServicePtr& service_ptr);
        void create_superadmin_endpoints(const api::ServicePtr& service_ptr);
        void create_authentication_endpoints(api::ServicePtr& service_ptr);
    };
}

