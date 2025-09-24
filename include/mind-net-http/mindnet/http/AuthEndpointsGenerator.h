//
// Created by robertvokac on 9/24/25.
//
#ifndef AUTHENDPOINTSGENERATOR_H
#define AUTHENDPOINTSGENERATOR_H


#include "crow.h"
#include "mindnet/api/IService.h"

namespace mindnet::http
{
    typedef std::function<void()> procedure_ptr;

    class AuthEndpointsGenerator
    {
    public:
        void create_auth_endpoints(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app
            );
    };
}


#endif // AUTHENDPOINTSGENERATOR_H
