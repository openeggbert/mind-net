//
// Created by robertvokac on 8/16/25.
//
#pragma once


#include "crow.h"
#include "mindnet/api/IService.hpp"

namespace mindnet::http
{
    class ModelEndpointGenerator
    {
    public:
        void create_model_endpoint(crow::SimpleApp& app, api::ServicePtr& service_ptr,
                                   model::ModelDefinition& def);
    };
}
