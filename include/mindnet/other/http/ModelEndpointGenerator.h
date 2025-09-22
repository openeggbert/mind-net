//
// Created by robertvokac on 8/16/25.
//
#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include "crow.h"
#include "mindnet/other/api/IService.h"

namespace mindnet::http
{
    class ModelEndpointGenerator
    {
    public:
        void create_model_endpoint(crow::SimpleApp& app, api::ServicePtr& service_ptr,
                                   model::ModelDefinition& def);
    };
}
#endif // MAPCONTROLLER_H
