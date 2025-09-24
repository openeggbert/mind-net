//
// Created by robertvokac on 9/24/25.
//
#ifndef MODELDEFINITIONENDPOINTSGENERATOR_H
#define MODELDEFINITIONENDPOINTSGENERATOR_H


#include "crow.h"
#include "mindnet/api/IService.h"

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

#endif // MODELDEFINITIONENDPOINTSGENERATOR_H
