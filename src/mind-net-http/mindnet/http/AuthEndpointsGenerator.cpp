//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/AuthEndpointsGenerator.h"

#include "mindnet/api/IService.h"
#include "mindnet/essential/Configuration.h"
#include "mindnet/http/HttpUtils.h"
#include "mindnet/http/UserCredentials.h"
#include "mindnet/plugins/core/models/User.h"

namespace mindnet::http
{
    using mindnet::essential::g_configuration;

    inline std::string hash_password(const std::string& pass)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(pass.c_str()), pass.size(), hash);

        std::ostringstream os;
        for (unsigned char i : hash)
            os << std::hex << std::setw(2) << std::setfill('0') << (int)i;

        return os.str();
    }

#include <random>
#include <string>

    //openssl rand -base64 32

    std::string generate_secret_key(size_t length = 32)
    {
        static const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "!@#$%^&*()-_=+[]{}<>?/|";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

        std::string key;
        key.reserve(length);
        for (size_t i = 0; i < length; i++)
        {
            key.push_back(charset[dist(gen)]);
        }
        return key;
    }

    string get_jwt_secret()
    {
        return g_configuration.jwt_secret;
    }

    void AuthEndpointsGenerator::create_auth_endpoints(
        const api::ServicePtr& service_ptr,
        crow::SimpleApp& crow_app
    )
    {
        CROW_ROUTE(crow_app, "/api/v1/auth/login").methods("POST"_method)([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()

            UserCredentials credentials = req;
            if (!credentials.error.empty())
            {
                return crow::response(400, credentials.error);
            }
            string error;
            orm::QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, credentials.username);
            api::LoginToken login_token{req};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (users.first.empty()) { return crow::response(401, "User does not exist."); }
            plugins::core::models::User user;
            user.from_values(users.first[0]);

            string expected_password_hash = user.password_hash;
            string returned_password_hash = hash_password(credentials.password);
            bool verified = expected_password_hash == returned_password_hash;
            if (verified)
            {
                using namespace std::chrono_literals;
                auto token = jwt::create()
                             .set_issuer("crow-app")
                             .set_type("JWT")
                             .set_payload_claim("username", jwt::claim(credentials.username))
                             .set_payload_claim("user_id", jwt::claim(std::to_string(user.get_id())))
                             .set_expires_at(std::chrono::system_clock::now() + 7 * 24h)
                             .sign(jwt::algorithm::hs256{get_jwt_secret()});

                crow::json::wvalue response;
                response["token"] = token;
                return crow::response{response};
            }
            else
            {
                return crow::response(401, "Username or password is not correct.");
            }
        });

        CROW_ROUTE(crow_app, "/api/v1/auth/register").methods("POST"_method)([=](const crow::request& req)
        {
            check_maintenance_mode()

            if (g_configuration.registration_mode == essential::RegistrationMode::AdminAddsUsers)
            {
                return crow::response{405, "Endpoint /register is disabled. Only admin can add new users."};
            }
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password"))
                return crow::response{400};

            std::string username = body["username"].s();
            std::string password = body["password"].s();
            std::string display_name = body["display_name"].s();
            std::string profile_text = body["profile_text"].s();
            std::string email = body["email"].s();

            //

            orm::QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, username);
            query_params.fields = {plugins::core::columns::UserColumns::USERNAME};
            api::LoginToken login_token{req};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (users.second.ko())
            {
                return crow::response(500, "Checking, if user already exists, failed. " + users.second.error);
            }
            if (!users.first.empty()) { return crow::response(409, "User already exists."); }
            //

            std::string hashed = hash_password(password);
            plugins::core::models::User user;
            user.username = username;
            user.password_hash = hashed;
            user.display_name = display_name;
            user.role = mindnet::essential::UserRole::Reader;
            user.profile_text = profile_text;
            user.last_login = 0;
            user.email = email;
            user.status = essential::UserStatus::Active;

            auto fields_ = user.to_values();
            auto create_result = service_ptr.get()->
                                             create(plugins::core::models::USER_DEFINITION, login_token, fields_);
            if (create_result.second.ko())
            {
                return crow::response{400, "Registration failed. " + create_result.second.error};
            }

            return crow::response{201, "Registration successful"};
        });

        CROW_ROUTE(crow_app, "/api/v1/protected")([](const crow::request& req)
        {
            check_maintenance_mode()

            api::LoginToken login_token{req};
            return crow::response(login_token.status, login_token.msg);
        });
    }
}
