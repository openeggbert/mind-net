//
// Created by robertvokac on 8/16/25.
//
#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H


#include "crow.h"
#include "mindnet/IService.h"

namespace mindnet::http
{
    class ModelEndpointGenerator
    {
    public:
        void create_model_endpoint(crow::SimpleApp& app, ServicePtr& service_ptr,
                             model::ModelDefinition& def);
    };
}
#endif // MAPCONTROLLER_H
