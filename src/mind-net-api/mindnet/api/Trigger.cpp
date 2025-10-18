//
// Created by robertvokac on 9/13/25.
//

#include "mindnet/api/Trigger.h"
#include "mindnet/api/Service.h"
//

namespace mindnet::api
{
    nlohmann::json Trigger::call_query(const std::string& query_name, nlohmann::json& request)
    {
        return service_ptr->call_query(query_name, request);
    }
}
