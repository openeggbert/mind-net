//
// Created by robertvokac on 9/24/25.
//
#ifndef INFOHEALTHENDPOINTSGENERATOR_H
#define INFOHEALTHENDPOINTSGENERATOR_H


#include "crow.h"
#include "mindnet/api/IService.h"

namespace mindnet::http
{
    class InfoHealthEndpointsGenerator
    {
    public:
        void create_info_health_endpoints(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app
        );
    };
}

#endif // INFOHEALTHENDPOINTSGENERATOR_H
