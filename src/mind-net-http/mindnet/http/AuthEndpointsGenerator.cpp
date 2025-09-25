//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/AuthEndpointsGenerator.h"

#include <random>
#include <string>

#include "mindnet/api/IService.h"
#include "mindnet/essential/Configuration.h"
#include "mindnet/http/HttpUtils.h"
#include "mindnet/http/UserCredentials.h"
#include "mindnet/plugins/core/models/AccessToken.h"
#include "mindnet/plugins/core/models/LoginSession.h"
#include "mindnet/plugins/core/models/RefreshToken.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/plugins/core/validators/AccessTokenValidator.h"
#include "mindnet/util/Utils.h"

namespace mindnet::http
{
    using mindnet::essential::g_configuration;


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

    void AuthEndpointsGenerator::create_auth_endpoints(
        api::ServicePtr& service_ptr,
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
            api::AccessTokenContext login_token{req, service_ptr};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (users.first.empty()) { return crow::response(401, "User does not exist."); }
            plugins::core::models::User user;
            user.from_values(users.first[0]);

            string expected_password_hash = user.password_hash;
            string returned_password_hash = util::Utils::hash_sha_256(credentials.password);
            bool verified = expected_password_hash == returned_password_hash;
            if (!verified)
            {
                return crow::response(401, "Username or password is not correct.");
            }

            // -------------------------------
            // 1. Token Generation
            // -------------------------------
            auto now = util::Utils::currentUnixTimestamp();

            auto access_exp = now + 15 * 60; // 15 minutes
            auto refresh_exp = now + 30 * 24 * 3600; // 30 days

            std::string raw_access = generate_secret_key(32);
            std::string raw_refresh = generate_secret_key(64);

            std::string access_hash = util::Utils::hash_sha_256(raw_access); // or SHA256
            std::string refresh_hash = util::Utils::hash_sha_256(raw_refresh); // or SHA256


            // -------------------------------
            // 2. Save to DB
            // -------------------------------
            int access_id = -1;
            {
                plugins::core::models::AccessToken access_token;

                access_token.user_id  = user.get_id();
                access_token.token_hash = access_hash;
                access_token.token_purpose = plugins::core::enums::TokenPurpose::Session;
                access_token.issued_at = now;
                access_token.expires_at = access_exp;
                access_token.allowed_operations = "*";
                auto access_token_values = access_token.to_values();
                access_token_values[1] = now;
                access_token_values[2] = now;

                auto result = service_ptr->create(
                    plugins::core::models::ACCESS_TOKEN_DEFINITION,
                    login_token,
                    access_token_values);
                if (result.second.ko())
                {
                    return crow::response(401, "Saving access token to the table access_token failed for this reason: " + result.second.error );
                }
                access_id = result.first;
            }

            int refresh_id = -1;
            {
                plugins::core::models::RefreshToken refresh_token;

                refresh_token.user_id = user.get_id();
                refresh_token.token_hash = refresh_hash;
                refresh_token.issued_at = now;
                refresh_token.expires_at = refresh_exp;

                auto refresh_token_values = refresh_token.to_values();
                refresh_token_values[1] = now;
                refresh_token_values[2] = now;

                auto result = service_ptr->create(
                    plugins::core::models::REFRESH_TOKEN_DEFINITION,
                    login_token,
                    refresh_token_values);
                if (result.second.ko())
                {
                    return crow::response(
                        401, "Saving refresh token to the table refresh_token failed for this reason: " + result.second.
                        error);
                }
                refresh_id = result.first;
            }

            {
                plugins::core::models::LoginSession login_session;

                login_session.user_id = user.get_id();
                login_session.access_token_id = access_id;
                login_session.refresh_token_id = refresh_id;
                login_session.issued_at = now;
                login_session.expires_at = refresh_exp;
                login_session.ip_address = req.remote_ip_address;
                login_session.user_agent = req.get_header_value("User-Agent");

                auto login_session_values = login_session.to_values();
                login_session_values[1] = now;
                login_session_values[2] = now;

                auto result = service_ptr->create(
                    plugins::core::models::LOGIN_SESSION_DEFINITION,
                    login_token,
                    login_session_values);
                if (result.second.ko())
                {
                    return crow::response(
                        401, "Saving login session to the table login_session failed for this reason: " + result.second.
                        error);
                }
                
            }

            // -------------------------------
            // 3. Return to client
            // -------------------------------
            crow::json::wvalue response;
            response["access_token"] = raw_access;
            response["expires_in"] = 900;           // 15 minut
            response["refresh_token"] = raw_refresh;
            response["refresh_expires_in"] = 2592000; // 30 dní
            return crow::response{200, response};


        });

        CROW_ROUTE(crow_app, "/api/v1/auth/logout").methods("POST"_method)([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()

            auto body = crow::json::load(req.body);
            if (!body || !body.has("refresh_token"))
            {
                return crow::response{400, "Missing refresh_token"};
            }

            std::string raw_refresh = body["refresh_token"].s();
            std::string refresh_hash = util::Utils::hash_sha_256(raw_refresh);

            auto now = util::Utils::currentUnixTimestamp();
            api::AccessTokenContext ctx{req, service_ptr};

            // 1. Find refresh token
            orm::QueryParams query;
            query.add_filter(plugins::core::columns::RefreshTokenColumns::TOKEN_HASH, refresh_hash);
            auto tokens = service_ptr->list(plugins::core::models::REFRESH_TOKEN_DEFINITION, ctx, query);
            if (tokens.second.ko())
            {
                return crow::response{500, "Listing tokens failed: "  + tokens.second.error};
            }
            if (tokens.first.empty())
            {
                return crow::response{401, "Invalid refresh_token"};
            }

            plugins::core::models::RefreshToken refresh;
            refresh.from_values(tokens.first[0]);

            // 2. Mark refresh token as revoked
            refresh.is_revoked = true;
            refresh.revoked_at = now;
            auto v = refresh.to_values();
            service_ptr->update(plugins::core::models::REFRESH_TOKEN_DEFINITION, ctx, refresh.get_id(),v);

            // 3. Mark login session as terminated
            orm::QueryParams session_query;
            session_query.add_filter(plugins::core::columns::LoginSessionColumns::REFRESH_TOKEN_ID,
                                     std::to_string(refresh.get_id()));
            auto sessions = service_ptr->list(plugins::core::models::LOGIN_SESSION_DEFINITION, ctx, session_query);
            for (auto& s : sessions.first)
            {
                plugins::core::models::LoginSession session;
                session.from_values(s);
                session.expires_at = now; // or session.is_revoked = 1, if you have the flag
                auto session_values = session.to_values();
                service_ptr->update(plugins::core::models::LOGIN_SESSION_DEFINITION, ctx, session.get_id(),session_values);
            }

            // 4. (optional) Mark access tokens as revoked
            orm::QueryParams access_query;
            access_query.add_filter(plugins::core::columns::AccessTokenColumns::USER_ID,
                                    std::to_string(refresh.user_id));
            auto accesses = service_ptr->list(plugins::core::models::ACCESS_TOKEN_DEFINITION, ctx, access_query);
            for (auto& a : accesses.first)
            {
                plugins::core::models::AccessToken access;
                access.from_values(a);
                access.is_revoked = true;
                access.revoked_at = now;
                auto access_token_values = access.to_values();
                service_ptr->update(plugins::core::models::ACCESS_TOKEN_DEFINITION, ctx, access.get_id(),access_token_values);
            }

            return crow::response{200, "Logout successful"};
        });

        CROW_ROUTE(crow_app, "/api/v1/auth/refresh_token").methods("POST"_method)(
            [service_ptr](const crow::request& req)
            {
                check_maintenance_mode()

                auto body = crow::json::load(req.body);
                if (!body || !body.has("refresh_token"))
                {
                    return crow::response{400, "Missing refresh_token"};
                }

                std::string raw_refresh = body["refresh_token"].s();
                std::string refresh_hash = util::Utils::hash_sha_256(raw_refresh);

                auto now = util::Utils::currentUnixTimestamp();
                api::AccessTokenContext ctx{req, service_ptr};

                // 1. Find refresh token
                orm::QueryParams query;
                query.add_filter(plugins::core::columns::RefreshTokenColumns::TOKEN_HASH, refresh_hash);
                auto result = service_ptr->list(plugins::core::models::REFRESH_TOKEN_DEFINITION, ctx, query);
                if (result.first.empty())
                {
                    return crow::response{401, "Invalid refresh_token"};
                }

                plugins::core::models::RefreshToken refresh;
                refresh.from_values(result.first[0]);

                // 2. Validation
                if (refresh.is_revoked || refresh.expires_at < now)
                {
                    return crow::response{401, "Refresh token expired or revoked"};
                }

                // 3. Generate new access token
                auto access_exp = now + 15 * 60; // 15 minutes
                std::string raw_access = generate_secret_key(32);
                std::string access_hash = util::Utils::hash_sha_256(raw_access);

                plugins::core::models::AccessToken access_token;
                access_token.user_id = refresh.user_id;
                access_token.token_hash = access_hash;
                access_token.token_purpose = plugins::core::enums::TokenPurpose::Session;
                access_token.issued_at = now;
                access_token.expires_at = access_exp;
                access_token.allowed_operations = "*";

                auto values = access_token.to_values();
                values[1] = now; // created_at
                values[2] = now; // updated_at

                auto create_res = service_ptr->create(
                    plugins::core::models::ACCESS_TOKEN_DEFINITION,
                    ctx,
                    values
                );

                if (create_res.second.ko())
                {
                    return crow::response{500, "Failed to create new access token: " + create_res.second.error};
                }

                // 4. Return to client
                crow::json::wvalue response;
                response["access_token"] = raw_access;
                response["expires_in"] = 900; // 15 minutes
                return crow::response{200, response};
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
            api::AccessTokenContext login_token{req, service_ptr};
            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, login_token, query_params);
            if (users.second.ko())
            {
                return crow::response(500, "Checking, if user already exists, failed. " + users.second.error);
            }
            if (!users.first.empty()) { return crow::response(409, "User already exists."); }
            //

            std::string hashed = util::Utils::hash_sha_256(password);
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

        CROW_ROUTE(crow_app, "/api/v1/protected")([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()

            api::AccessTokenContext login_token{req, service_ptr};
            return crow::response(login_token.status, login_token.msg);
        });
    }
}
