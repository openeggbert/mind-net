//
// Created by robertvokac on 9/7/25.
//
#include "mindnet/api/Job.hpp"
#include "mindnet/api/Service.hpp"

namespace mindnet::api
{
    nlohmann::json mindnet::api::Job::call_query(const std::string& query_name, nlohmann::json& request)
    {
        return service_ptr->call_query(query_name, request);
    }
}
