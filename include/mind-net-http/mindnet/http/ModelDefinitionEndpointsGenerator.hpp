//
// Created by robertvokac on 9/24/25.
//
#pragma once



#include "crow.h"
#include "mindnet/api/IService.hpp"

namespace mindnet::http
{
    class ModelDefinitionEndpointsGenerator
    {
    public:
        void create_model_definition_endpoints(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app
        );
    };
}

