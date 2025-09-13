//
// Created by robertvokac on 9/5/25.
//

#ifndef MIND_NET_USERCREDENTIALS_H
#define MIND_NET_USERCREDENTIALS_H
#include "crow/http_request.h"

namespace mindnet::http
{
    struct UserCredentials
    {
        std::string username;
        std::string password;
        std::string error;

        UserCredentials(const crow::request& req)
        {
            auto auth = req.get_header_value("Authorization"); // Basic base64(username:password)
            if (auth.empty())
            {
                error = "Missing Authorization header.";
                return;
            }
            if (auth.rfind("Basic ", 0) != 0)
            {
                error = "Invalid Authorization header.";
                return;
            }

            std::string creds;
            try
            {
                creds = crow::utility::base64decode(auth.substr(6));
            }
            catch (const std::exception& e)
            {
                error = "Invalid base64 encoding in Authorization header.";
                return;
            }

            auto sep = creds.find(':');
            if (sep == std::string::npos)
            {
                error = "Invalid credentials format.";
                return;
            }

            username = creds.substr(0, sep);
            password = creds.substr(sep + 1);
        }
    };
}

#endif //MIND_NET_USERCREDENTIALS_H
