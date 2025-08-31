//
// Created by robertvokac on 8/16/25.
//
#ifndef HTTPMETHOD_H
#define HTTPMETHOD_H
#include <string>

namespace mindnet::http
{
    enum class HttpMethod
    {
        GET,
        POST,
        PUT,
        DELETE,
        PATCH,
        OPTIONS,
        HEAD
    };

    inline const std::string& http_method_to_string(HttpMethod& method)
    {
        static const std::string methodNames[] = {
            "GET", "POST", "PUT", "DELETE", "PATCH", "OPTIONS", "HEAD"
        };

        static const std::string unknown = "UNKNOWN";

        size_t index = static_cast<size_t>(method);
        if (index < sizeof(methodNames) / sizeof(methodNames[0]))
        {
            return methodNames[index];
        }
        return unknown;
    }

    inline HttpMethod string_to_http_method(const std::string& method_str)
    {
        static const std::string methodNames[] = {
            "GET", "POST", "PUT", "DELETE", "PATCH", "OPTIONS", "HEAD"
        };

        for (size_t i = 0; i < sizeof(methodNames) / sizeof(methodNames[0]); i++)
        {
            if (methodNames[i] == method_str)
            {
                return static_cast<HttpMethod>(i);
            }
        }

        return HttpMethod::GET; // Default to GET if not found
    }

    inline HttpMethod crudl_to_http_method(const enums::Crudl crudl)
    {
        switch (crudl)
        {
        case enums::Crudl::CREATE:
            return HttpMethod::POST;
        case enums::Crudl::READ:
            return HttpMethod::GET;
        case enums::Crudl::UPDATE:
            return HttpMethod::PUT;
        case enums::Crudl::DELETE:
            return HttpMethod::DELETE;
        case enums::Crudl::LIST:
            return HttpMethod::GET;
        default:
            return HttpMethod::GET;
        }
    }

} // namespace mindnet::http
#endif // HTTPMETHOD_H
