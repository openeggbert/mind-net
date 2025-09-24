//
// Created by robertvokac on 9/24/25.
//
#ifndef WEBENDPOINTSGENERATOR_H
#define WEBENDPOINTSGENERATOR_H


#include "crow.h"
#include "mindnet/api/IService.h"

namespace mindnet::http
{
    typedef std::function<void()> procedure_ptr;

    struct CachedFile
    {
        std::string content;
        std::filesystem::file_time_type last_modified;
    };

    class WebEndpointsGenerator
    {
    public:
        WebEndpointsGenerator(const std::string& directory_for_static_files_);
        void create_web_endpoints(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app
            );

    private:
        std::unordered_map<std::string, CachedFile> file_cache;
        std::string directory_for_static_files;
    };
}

#endif // WEBENDPOINTSGENERATOR_H
