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
        bool system{false};

        AccessTokenContext(int user_id, const std::string& msg, int status, bool system = false);

        AccessTokenContext(const crow::request& req, ServicePtr service_ptr);

        bool ok() const { return status == 200; }
        bool ko() const { return !ok(); }

        bool is_system() const
        {
            if (system) return true;
            return user_id == 0 && msg == "system" && status == 403;
        }

        bool is_not_system() const
        {
            return !is_system();
        }
    };
}

#endif // ACCESSTOKENCONTEXT_H
