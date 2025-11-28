/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/api/AccessTokenContext.hpp"

#include "crow/http_request.h"
#include "mindnet/api/IService.hpp"

#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/plugins/core/columns/AccessTokenColumns.hpp"
#include "mindnet/plugins/core/models/AccessToken.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::api
{
    AccessTokenContext::AccessTokenContext(identification user_id, const std::string& msg, int status, bool system)
        : user_id(user_id),
          msg(msg),
          status(status),
          system(system)
    {
    }

    AccessTokenContext::AccessTokenContext(const crow::request& req, ServicePtr service_ptr)
    {
        auto auth = req.get_header_value("Authorization");
        if (auth.empty() || auth.rfind("Bearer ", 0) != 0)
        {
            status = 401;
            msg = "Invalid or missing Authorization header";
            return;
        }
        std::string raw_token = auth.substr(7);
        std::string token_hash = util::Utils::compute_sha256(raw_token);

        orm::QueryParams q;
        q.add_filter(plugins::core::columns::AccessTokenColumns::TOKEN_HASH, token_hash);

        api::AccessTokenContext system_token{0, "system", 403};
        auto tokens = service_ptr->list(plugins::core::models::ACCESS_TOKEN_DEFINITION, system_token, q, 0);
        if (tokens.second.ko())
        {
            status = 500;
            msg = "Listing tokens failed.";
            return;
        }
        if (tokens.first.empty())
        {
            status = 401;
            msg = "Access token invalid or revoked";
            return;
        }

        auto token_row = tokens.first[0];
        plugins::core::models::AccessToken access_token;
        access_token.from_values(token_row);

        if (access_token.is_revoked)
        {
            status = 401;
            msg = "Access token revoked";
            return;
        }
        if (util::Utils::current_unix_timestamp_ms() >= access_token.expires_at)
        {
            status = 401;
            msg = "Access token expired";
            return;
        }

        {
            access_token.last_used_at = util::Utils::current_unix_timestamp_ms();
            access_token.ip_address = req.remote_ip_address;
            access_token.user_agent = req.get_header_value("User-Agent");

            auto v = access_token.to_values();

            // v.erase(v.begin(),
            //              v.begin() + std::min<size_t>(2, v.size()));
            // v.push_back(access_token.get_id());

            auto updated = service_ptr->update(plugins::core::models::ACCESS_TOKEN_DEFINITION, system_token,
                                               access_token.get_id(), v, 0);
            if (updated.ko())
            {
                status = 500;
                msg = "Access token update failed: " + updated.error;
                return;
            }
        }
        user_id = access_token.user_id;
        status = 200;
        msg = "Welcome";
    }
}
