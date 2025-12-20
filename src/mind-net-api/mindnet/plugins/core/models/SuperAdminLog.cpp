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

#include "mindnet/plugins/core/models/SuperAdminLog.hpp"

namespace mindnet::plugins::core::models
{
    create_model_cpp_methods(SuperAdminLog)

    string SuperAdminLog::validate()
    {
        using columns::SuperAdminLogColumns;

        validator_chain_vector list{
            [this] { return test_at_least(user_id, 0, SuperAdminLogColumns::USER_ID); },
            [this] { return testt_not_empty(endpoint, SuperAdminLogColumns::ENDPOINT); },
            [this] { return testt_not_empty(action, SuperAdminLogColumns::ACTION); },
            [this] { return test_ne(cast64(method), 0, SuperAdminLogColumns::METHOD); },
            [this] { return test_true(status_code > 0, "Status code must be positive"); },
            [this] { return testt_at_most(error, 256, SuperAdminLogColumns::ERROR); },
        };
        return util::ValidatorChain::run(list);
    }

    static std::string serializeParamsForSuperAdmin(const crow::query_string& qs)
    {
        std::ostringstream oss;
        bool first = true;
        for (auto& key : qs.keys())
        {
            auto value = qs.get(key);
            if (!first) oss << "&";
            oss << key << "=" << value;
            first = false;
        }
        return oss.str();
    }

    const SuperAdminLog super_admin_log_from_crow_request(
        const crow::request& req,
        int user_id,
        int status_code,
        const std::string& action,
        std::string error,
        const std::string& diff)
    {
        SuperAdminLog log;

        log.user_id = user_id;
        log.ip_address = req.remote_ip_address;
        log.user_agent = req.get_header_value("User-Agent");
        log.endpoint = req.url;
        log.action = action;

        switch (req.method)
        {
        case crow::HTTPMethod::Get: log.method = enums::HttpMethod::GET;
            break;
        case crow::HTTPMethod::Post: log.method = enums::HttpMethod::POST;
            break;
        case crow::HTTPMethod::Put: log.method = enums::HttpMethod::PUT;
            break;
        case crow::HTTPMethod::Delete: log.method = enums::HttpMethod::DELETE;
            break;
        case crow::HTTPMethod::Patch: log.method = enums::HttpMethod::PATCH;
            break;
        default: log.method = enums::HttpMethod::Undefined;
            break;
        }

        log.parameters = serializeParamsForSuperAdmin(req.url_params);
        log.request_body = req.body;
        log.diff = diff;
        log.status_code = status_code;
        log.error = error;
        log.success = status_code == 303 || (status_code >= 200 && status_code < 300);

        return log;
    }
}