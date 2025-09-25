//
// Created by robertvokac on 9/5/25.
//
#ifndef ACCESSTOKENCONTEXT_H
#define ACCESSTOKENCONTEXT_H


#include "crow/http_request.h"

namespace mindnet::api
{
    class IService;
    using ServicePtr = std::shared_ptr<IService>;

    struct AccessTokenContext
    {
        int user_id{};
        std::string msg;
        int status{};

        AccessTokenContext(int user_id, const std::string& msg, int status);

        AccessTokenContext(const crow::request& req, ServicePtr service_ptr);

        bool ok() const { return status == 200; }
        bool ko() { return !ok(); }
    };
}

#endif // ACCESSTOKENCONTEXT_H
