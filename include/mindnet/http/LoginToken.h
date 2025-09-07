//
// Created by robertvokac on 9/5/25.
//
#ifndef LOGINTOKEN_H
#define LOGINTOKEN_H

#include "crow/http_request.h"
#include "jwt-cpp/jwt.h"
#include "mindnet/Configuration.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::http
{
    struct LoginToken
    {
        std::string username;
        int user_id;
        std::string msg;
        int status{};

        LoginToken(const crow::request& req)
        {
            auto auth = req.get_header_value("Authorization"); // Bearer <token>
            if (auth.empty() || auth.rfind("Bearer ", 0) != 0)
            {
                status = 401;
                msg = "Invalid or missing Authorization header";
                return;
            }
            std::string token = auth.substr(7);
            if (token.empty())
            {
                status = 401;
                msg = "Missing token";
                return;
            }

            try
            {
                auto decoded = jwt::decode(token);
                auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{g_configuration.jwt_secret})
                                .with_issuer("crow-app");

                verifier.verify(decoded);

                username = decoded.get_payload_claim("username").as_string();
                user_id = stoi(decoded.get_payload_claim("user_id").as_string());
                status = 200;
                msg = "Welcome, " + username;
                return;
            }
            catch (const std::exception& e)
            {
                status = 403;
                msg = "Invalid token";
                return;
            }
        }
        bool ok() {return status == 200;}
        bool ko() {return !ok();}

    };
}


#endif // LOGINTOKEN_H
