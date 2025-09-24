//
// Created by robertvokac on 9/24/25.
//

#ifndef MIND_NET_SUPERADMINENDPOINTSGENERATOR_H
#define MIND_NET_SUPERADMINENDPOINTSGENERATOR_H

#include "crow.h"
#include "mindnet/api/IService.h"

namespace mindnet::http
{
    typedef std::function<void()> procedure_ptr;

    class SuperAdminEndpointsGenerator
    {
    public:
        void create_superadmin_endpoints(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app,
            procedure_ptr request_restart,
            procedure_ptr request_shutdown
            );
    };
}


#endif //MIND_NET_SUPERADMINENDPOINTSGENERATOR_H