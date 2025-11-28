/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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

