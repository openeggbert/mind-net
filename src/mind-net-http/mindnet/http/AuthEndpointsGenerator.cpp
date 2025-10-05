//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/AuthEndpointsGenerator.h"

#include <string>

#include "mindnet/api/IService.h"
#include "mindnet/essential/Configuration.h"
#include "mindnet/essential/Global.h"
#include "mindnet/http/HttpUtils.h"
#include "mindnet/http/UserCredentials.h"
#include "mindnet/plugins/core/models/AccessToken.h"
#include "mindnet/plugins/core/models/AuthLog.h"
#include "mindnet/plugins/core/models/LoginSession.h"
#include "mindnet/plugins/core/models/RefreshToken.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/plugins/core/validators/AccessTokenValidator.h"
#include "mindnet/util/Utils.h"

namespace mindnet::http
{
    using mindnet::essential::g_configuration;

    crow::json::wvalue mask_sensitive(const crow::json::rvalue& body) {
        crow::json::wvalue safe;

        static const std::unordered_set<std::string> sensitive_keys = {
            "password", "old_password", "new_password",
            "refresh_token", "access_token"
        };

        for (auto& key : body.keys()) {
            if (sensitive_keys.count(key)) {
                safe[key] = "***";
            } else {
                safe[key] = body[key];
            }
        }

        return safe;
    }

    void AuthEndpointsGenerator::create_auth_endpoints(
        api::ServicePtr& service_ptr,
        crow::SimpleApp& crow_app
    )
    {
            auto log_request = [](
                const api::ServicePtr& service_ptr,
                const crow::request& req,
                api::AccessTokenContext& login_token,
                int status_code,
                const int& entity_id = 0,
                const std::string& error = ""
            )
            {
                auto body = crow::json::load(req.body);
                std::string masked_request_body;
                if (!body) {
                    // request body is not a valid JSON
                } else {
                    auto safe = mask_sensitive(body);
                    masked_request_body = safe.dump();
                }

                auto log_object = plugins::core::models::auth_log_from_crow_request(
                    req,
                    masked_request_body,
                    login_token.user_id,
                    status_code,
                    entity_id,
                    error);
                auto log = log_object.to_values();
                int64_t now = static_cast<int64_t>(mindnet::util::Utils::currentUnixTimestamp());
                log[1] = now;
                log[2] = now;

                if (service_ptr == nullptr)
                {
                    essential::warn <<
                        "Saving record to the table auth_log failed for this reason: service_ptr == nullptr " <<
                        log_object.to_json() << essential::commit;
                    return;
                }

                auto result = service_ptr->create(
                    plugins::core::models::AUTH_LOG_DEFINITION,
                    login_token,
                    log);
                if (result.second.ko())
                {
                    essential::warn << "Saving record to the table auth_log failed for this reason: " << result.second.error <<
                        log_object.to_json() << essential::commit;
                }
            };

        CROW_ROUTE(crow_app, "/api/v1/auth/login").methods("POST"_method)([service_ptr, &log_request](const crow::request& req)
        {
            check_maintenance_mode()

            auto body = crow::json::load(req.body);
            api::AccessTokenContext system_token{0, "system", 403};\
            if (!body || !body.has("username") || !body.has("password"))
            {
                log_request(service_ptr, req, system_token, 403, 0 , "Missing username or password");
                return crow::response(400, "Missing username or password");
            }

            std::string username = body["username"].s();
            std::string password = body["password"].s();

            orm::QueryParams query_params;
            query_params.add_filter(plugins::core::columns::UserColumns::USERNAME, username);

            auto users = service_ptr.get()->list(plugins::core::models::USER_DEFINITION, system_token, query_params);
            if (users.second.ko())
            {
                log_request(service_ptr, req, system_token, 500, 0 , "Loading list of users failed. " + users.second.error);
                return crow::response(500, "Loading list of users failed. " + users.second.error);
            }
            if (users.first.empty()) {
                log_request(service_ptr, req, system_token, 401, 0, "User does not exist.");
                return crow::response(401, "User does not exist.");
            }
            plugins::core::models::User user;
            user.from_values(users.first[0]);

            string expected_password_hash = user.password_hash;
            string returned_password_hash = util::Utils::hash_sha_256(password);
            bool verified = expected_password_hash == returned_password_hash;
            if (!verified)
            {
                log_request(service_ptr, req, system_token, 401, 0 , "Username or password is not correct.");
                return crow::response(401, "Username or password is not correct.");
            }

            // -------------------------------
            // 1. Token Generation
            // -------------------------------
            auto now = util::Utils::currentUnixTimestamp();

            auto access_exp = now + g_configuration.access_token_expires_in * 60;
            auto refresh_exp = now + g_configuration.refresh_token_expires_in * 60;

            std::string raw_access = util::Utils::generate_secret_key(32);
            std::string raw_refresh = util::Utils::generate_secret_key(64);

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
                    system_token,
                    access_token_values);
                if (result.second.ko())
                {
                    log_request(service_ptr, req, system_token, 401, 0 , "Saving access token to the table access_token failed for this reason: " + result.second.error);
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
                    system_token,
                    refresh_token_values);
                if (result.second.ko())
                {
                    log_request(service_ptr, req, system_token, 401, 0 , "Saving refresh token to the table access_token failed for this reason: " + result.second.error);
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
                    system_token,
                    login_session_values);
                if (result.second.ko())
                {
                    log_request(service_ptr, req, system_token, 401, 0,
                                "Saving refresh token to the table login_session failed for this reason: " + result.
                                second.error);

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
            response["access_token_expires_at"] = access_exp;
            response["refresh_token"] = raw_refresh;
            response["refresh_token_expires_at"] = refresh_exp;

            log_request(service_ptr, req, system_token, 200);
            return crow::response{200, response};


        });

        CROW_ROUTE(crow_app, "/api/v1/auth/logout").methods("POST"_method)([service_ptr, &log_request](const crow::request& req)
        {
            check_maintenance_mode()

            auto body = crow::json::load(req.body);

            api::AccessTokenContext ctx{req, service_ptr};
            api::AccessTokenContext system_token{0, "system", 403};
            api::AccessTokenContext& token_to_be_used = ctx.ok() ? ctx : system_token;

            if (!body || !body.has("refresh_token"))
            {
                log_request(service_ptr, req, token_to_be_used, 400, 0, "Missing refresh_token");
                return crow::response{400, "Missing refresh_token"};
            }

            std::string raw_refresh = body["refresh_token"].s();
            std::string refresh_hash = util::Utils::hash_sha_256(raw_refresh);

            auto now = util::Utils::currentUnixTimestamp();

            // 1. Find refresh token
            orm::QueryParams query;
            query.add_filter(plugins::core::columns::RefreshTokenColumns::TOKEN_HASH, refresh_hash);
            auto tokens = service_ptr->list(plugins::core::models::REFRESH_TOKEN_DEFINITION, token_to_be_used, query);
            if (tokens.second.ko())
            {
                log_request(service_ptr, req, token_to_be_used, 500, 0, "Listing tokens failed: " + tokens.second.error);
                return crow::response{500, "Listing tokens failed: "  + tokens.second.error};
            }
            if (tokens.first.empty())
            {
                log_request(service_ptr, req, token_to_be_used, 401, 0, "Invalid refresh_token");
                return crow::response{401, "Invalid refresh_token"};
            }

            plugins::core::models::RefreshToken refresh;
            refresh.from_values(tokens.first[0]);

            // 2. Mark refresh token as revoked
            refresh.is_revoked = true;
            refresh.revoked_at = now;
            auto v = refresh.to_values();
            auto refresh_updated = service_ptr->update(plugins::core::models::REFRESH_TOKEN_DEFINITION, token_to_be_used, refresh.get_id(),v);
            if (refresh_updated.ko())
            {
                log_request(service_ptr, req, token_to_be_used, 500, 0, "Update of refresh token failed: " + refresh_updated.error);
                return crow::response{500, "Update of refresh token failed: " + refresh_updated.error};
            }

            // 3. Mark login session as terminated
            orm::QueryParams session_query;
            session_query.add_filter(plugins::core::columns::LoginSessionColumns::REFRESH_TOKEN_ID,
                                     std::to_string(refresh.get_id()));
            auto sessions_listed = service_ptr->list(plugins::core::models::LOGIN_SESSION_DEFINITION, token_to_be_used,
                                                     session_query);
            if (sessions_listed.second.ko())
            {
                log_request(service_ptr, req, token_to_be_used, 500, 0,
                            "Listing login sessions failed: " + sessions_listed.second.error);
                return crow::response{500, "Listing login sessions failed: " + sessions_listed.second.error};
            }
            for (auto& s : sessions_listed.first)
            {
                plugins::core::models::LoginSession session;
                session.from_values(s);
                session.expires_at = now; // or session.is_revoked = 1, if you have the flag
                auto session_values = session.to_values();
                auto session_updated = service_ptr->update(plugins::core::models::LOGIN_SESSION_DEFINITION, token_to_be_used, session.get_id(),session_values);

                if (session_updated.ko())
                {
                    log_request(service_ptr, req, token_to_be_used, 500, 0, "Update of login session failed: " + session_updated.error);
                    return crow::response{500, "Update of login session failed: " + session_updated.error};
                }
            }

            // 4. (optional) Mark access tokens as revoked
            orm::QueryParams access_query;
            access_query.add_filter(plugins::core::columns::AccessTokenColumns::USER_ID,
                                    std::to_string(refresh.user_id));
            auto accesses_listed = service_ptr->list(plugins::core::models::ACCESS_TOKEN_DEFINITION, token_to_be_used, access_query);
            if (accesses_listed.second.ko())
            {
                log_request(service_ptr, req, token_to_be_used, 500, 0,
                            "Listing access tokens failed: " + accesses_listed.second.error);
                return crow::response{500, "Listing access tokens failed: " + sessions_listed.second.error};
            }
            for (auto& a : accesses_listed.first)
            {
                plugins::core::models::AccessToken access;
                access.from_values(a);
                access.is_revoked = true;
                access.revoked_at = now;
                auto access_token_values = access.to_values();
                auto access_updated = service_ptr->update(plugins::core::models::ACCESS_TOKEN_DEFINITION, token_to_be_used, access.get_id(),access_token_values);

                if (access_updated.ko())
                {
                    log_request(service_ptr, req, token_to_be_used, 500, 0 ,"Update of access token failed: " + access_updated.error);
                    return crow::response{500, "Update of access token failed: " + access_updated.error};
                }
            }

            log_request(service_ptr, req, token_to_be_used, 200, 0, "");
            return crow::response{200, "Logout successful"};
        });

        CROW_ROUTE(crow_app, "/api/v1/auth/refresh_token").methods("POST"_method)(
            [service_ptr, &log_request](const crow::request& req)
            {
                check_maintenance_mode()

                auto body = crow::json::load(req.body);
                //api::AccessTokenContext ctx{req, service_ptr};
                api::AccessTokenContext system_token{0, "system", 403};
                if (!body || !body.has("refresh_token"))
                {
                    log_request(service_ptr, req, system_token, 400, 0, "Missing refresh_token");
                    return crow::response{400, "Missing refresh_token"};
                }

                std::string raw_refresh = body["refresh_token"].s();
                std::string refresh_hash = util::Utils::hash_sha_256(raw_refresh);

                auto now = util::Utils::currentUnixTimestamp();

                // 1. Find refresh token
                orm::QueryParams query;
                query.add_filter(plugins::core::columns::RefreshTokenColumns::TOKEN_HASH, refresh_hash);
                auto result = service_ptr->list(plugins::core::models::REFRESH_TOKEN_DEFINITION, system_token, query);
                if (result.second.ko())
                {
                    log_request(service_ptr, req, system_token, result.second.status, 0, result.second.error);
                    return crow::response{result.second.status, result.second.error};
                }
                if (result.first.empty())
                {
                    log_request(service_ptr, req, system_token, 401, 0, "Invalid refresh_token");
                    return crow::response{401, "Invalid refresh_token"};
                }

                plugins::core::models::RefreshToken refresh;
                refresh.from_values(result.first[0]);

                // 2. Validation
                if (refresh.is_revoked || refresh.expires_at < now)
                {
                    log_request(service_ptr, req, system_token, 401, 0, "Refresh token expired or revoked");
                    return crow::response{401, "Refresh token expired or revoked"};
                }

                // 3. Generate new access token
                auto access_exp = now + 15 * 60; // 15 minutes
                std::string raw_access = util::Utils::generate_secret_key(32);
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
                    system_token,
                    values
                );

                if (create_res.second.ko())
                {
                    log_request(service_ptr, req, system_token, 500, 0,
                                "Failed to create new access token: " + create_res.second.error);
                    return crow::response{500, "Failed to create new access token: " + create_res.second.error};
                }

                // 4. Decide if we need refresh token rotation
                int64_t seconds_left = refresh.expires_at - now;
                int64_t rotation_threshold = g_configuration.refresh_token_rotation_threshold_in * 60;
                bool need_rotation = seconds_left <= rotation_threshold;

                crow::json::wvalue response;
                response["access_token"] = raw_access;
                response["access_token_expires_at"] = access_exp;

                if (need_rotation)
                {
                    // revoke old refresh
                    refresh.is_revoked = true;
                    refresh.revoked_at = now;

                    auto old_vals = refresh.to_values();
                    old_vals[2] = now;
                    auto upd = service_ptr->update(
                        plugins::core::models::REFRESH_TOKEN_DEFINITION,
                        system_token,
                        refresh.get_id(),
                        old_vals
                    );
                    if (upd.ko())
                    {
                        log_request(service_ptr, req, system_token, 500, 0,
                                    "Failed to revoke old refresh token: " + upd.error);
                        return crow::response{500, "Failed to revoke old refresh token: " + upd.error};
                    }

                    // create new refresh
                    std::string raw_refresh = util::Utils::generate_secret_key(64);
                    std::string refresh_hash_new = util::Utils::hash_sha_256(raw_refresh);

                    plugins::core::models::RefreshToken new_refresh;
                    new_refresh.user_id = refresh.user_id;
                    new_refresh.token_hash = refresh_hash_new;
                    new_refresh.issued_at = now;
                    new_refresh.expires_at = now + 30 * 24 * 3600;
                    new_refresh.rotated_from_id = refresh.get_id();
                    new_refresh.last_used_at = now;
                    new_refresh.ip_address = req.remote_ip_address;
                    new_refresh.user_agent = req.get_header_value("User-Agent");

                    auto new_vals = new_refresh.to_values();
                    new_vals[1] = now;
                    new_vals[2] = now;

                    auto new_res = service_ptr->create(
                        plugins::core::models::REFRESH_TOKEN_DEFINITION,
                        system_token,
                        new_vals
                    );
                    if (new_res.second.ko())
                    {
                        log_request(service_ptr, req, system_token, 500, 0,
                                    "Failed to create new refresh token: " + new_res.second.error);
                        return crow::response{500, "Failed to create new refresh token: " + new_res.second.error};
                    }
                    int new_id = new_res.first;

                    // link old refresh to new one
                    refresh.replaced_by_id = new_id;
                    auto link_vals = refresh.to_values();
                    service_ptr->update(
                        plugins::core::models::REFRESH_TOKEN_DEFINITION,
                        system_token,
                        refresh.get_id(),
                        link_vals
                    );

                    // return new refresh token to client
                    response["refresh_token"] = raw_refresh;
                    response["refresh_token_expires_at"] = new_refresh.expires_at;
                }
                else
                {
                    // sliding update only
                    refresh.last_used_at = now;
                    refresh.ip_address = req.remote_ip_address;
                    refresh.user_agent = req.get_header_value("User-Agent");
                    auto rv = refresh.to_values();
                    rv[2] = now;
                    service_ptr->update(
                        plugins::core::models::REFRESH_TOKEN_DEFINITION,
                        system_token,
                        refresh.get_id(),
                        rv
                    );
                    // no refresh token returned to client
                }

                log_request(service_ptr, req, system_token, 200, 0, "");
                return crow::response{200, response};

            });


        CROW_ROUTE(crow_app, "/api/v1/auth/register").methods("POST"_method)([service_ptr, &log_request](const crow::request& req)
        {
            check_maintenance_mode()
            api::AccessTokenContext ctx{req, service_ptr};

            if (g_configuration.registration_mode == essential::RegistrationMode::AdminAddsUsers)
            {
                log_request(service_ptr, req, ctx, 405, 0, "Endpoint /register is disabled. Only admin can add new users.");
                return crow::response{405, "Endpoint /register is disabled. Only admin can add new users."};
            }
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                log_request(service_ptr, req, ctx, 400, 0, "Missing username or password");
                return crow::response{400};
            }

            std::string username = body["username"].s();
            std::string password = body["password"].s();
            if (username == password)
            {
                log_request(service_ptr, req, ctx, 400, 0, "Password must be different from username.");
                return crow::response{400, "Password must be different from username."};
            }
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
                log_request(service_ptr, req, ctx, 500, 0, "Checking, if user already exists, failed. " + users.second.error);
                return crow::response(500, "Checking, if user already exists, failed. " + users.second.error);
            }
            if (!users.first.empty()) {
                log_request(service_ptr, req, ctx, 409, 0, "User already exists.");
                return crow::response(409, "User already exists.");
            }
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
                log_request(service_ptr, req, ctx, 400, 0, "Registration failed. " + create_result.second.error);
                return crow::response{400, "Registration failed. " + create_result.second.error};
            }

            log_request(service_ptr, req, ctx, 201, create_result.first, "");
            return crow::response{201, "Registration successful"};
        });

        CROW_ROUTE(crow_app, "/api/v1/auth/change_password").methods("POST"_method)(
    [service_ptr, &log_request](const crow::request& req)
    {
        check_maintenance_mode()

        api::AccessTokenContext ctx{req, service_ptr};
        if (ctx.status != 200)
        {
            log_request(service_ptr, req, ctx, ctx.status, 0, ctx.msg);
            return crow::response{ctx.status, ctx.msg};
        }

        auto body = crow::json::load(req.body);
        if (!body || !body.has("old_password") || !body.has("new_password"))
        {
            log_request(service_ptr, req, ctx, 400, 0, "Missing old_password or new_password");
            return crow::response{400, "Missing old_password or new_password"};
        }

        std::string old_password = body["old_password"].s();
        std::string new_password = body["new_password"].s();

        if (old_password == new_password)
        {
            log_request(service_ptr, req, ctx, 400, 0, "New password must be different from old password");
            return crow::response{400, "New password must be different from old password"};
        }

        // 1. Load user
        auto user_id = ctx.user_id;
        auto user_res = service_ptr->read(plugins::core::models::USER_DEFINITION, ctx, user_id);
        if (user_res.second.ko())
        {
            log_request(service_ptr, req, ctx, 500, 0, "Failed to load user: " + user_res.second.error);
            return crow::response{500, "Failed to load user: " + user_res.second.error};
        }
        if (user_res.first.empty())
        {
            log_request(service_ptr, req, ctx, 404, 0, "User not found");
            return crow::response{404, "User not found"};
        }

        plugins::core::models::User user;
        user.from_values(user_res.first);

        if (user.username == new_password)
        {
            log_request(service_ptr, req, ctx, 400, 0, "Password must be different from username");
            return crow::response{400, "Password must be different from username."};
        }

        // 2. Verify old password
        std::string old_hash = util::Utils::hash_sha_256(old_password);
        if (user.password_hash != old_hash)
        {
            log_request(service_ptr, req, ctx, 401, 0, "Old password is incorrect");
            return crow::response{401, "Old password is incorrect"};
        }

        // 3. Save new password
        std::string new_hash = util::Utils::hash_sha_256(new_password);
        user.password_hash = new_hash;
        auto v = user.to_values();
        ctx.system = true;
        auto update_res = service_ptr->update(
            plugins::core::models::USER_DEFINITION,
            ctx,
            user.get_id(),
            v
        );
        ctx.system = false;

        if (update_res.ko())
        {
            log_request(service_ptr, req, ctx, 500, 0, "Failed to update password: " + update_res.error);
            return crow::response{500, "Failed to update password: " + update_res.error};
        }

        // 4. (Optional) Revoke all sessions/tokens for this user
        orm::QueryParams refresh_query;
        refresh_query.add_filter(plugins::core::columns::RefreshTokenColumns::USER_ID, std::to_string(user.get_id()));
        auto refresh_tokens = service_ptr->list(plugins::core::models::REFRESH_TOKEN_DEFINITION, ctx, refresh_query);
        auto now = util::Utils::currentUnixTimestamp();
        for (auto& r : refresh_tokens.first)
        {
            plugins::core::models::RefreshToken t;
            t.from_values(r);
            t.is_revoked = true;
            t.revoked_at = now;
            auto tv = t.to_values();
            service_ptr->update(plugins::core::models::REFRESH_TOKEN_DEFINITION, ctx, t.get_id(), tv);
        }

        log_request(service_ptr, req, ctx, 200, 0, "");
        return crow::response{200, "Password changed successfully"};
    });


        // CROW_ROUTE(crow_app, "/api/v1/auth/protected")([service_ptr, &log_request](const crow::request& req)
        // {
        //     check_maintenance_mode()
        //
        //     api::AccessTokenContext login_token{req, service_ptr};
        //     log_request(service_ptr, req, login_token, login_token.status, 0, login_token.msg);
        //     return crow::response(login_token.status, login_token.msg);
        // });
    }
}