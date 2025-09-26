//
// Created by robertvokac on 9/21/25.
//

#include "mindnet/plugins/core/models/AuthLog.h"

namespace mindnet::plugins::core::models
{
    entity_fields AuthLog::to_values() const
    {
        entity_fields result;
        result.emplace_back(id);
        result.emplace_back(cast64(created_at));
        result.emplace_back(cast64(updated_at));
        result.emplace_back(cast64(user_id));
        result.emplace_back(ip_address);
        result.emplace_back(user_agent);
        result.emplace_back(endpoint);
        result.emplace_back(cast64(method)); // enum HttpMethod stored as number
        result.emplace_back(action);
        result.emplace_back(entity_name);
        result.emplace_back(cast64(entity_id));
        result.emplace_back(parameters);
        result.emplace_back(request_body);
        result.emplace_back(cast64(status_code));
        result.emplace_back(error);
        result.emplace_back(success);
        return result;
    }

    void AuthLog::from_values(const entity_fields& values)
    {
        int i = 0;

        auto number = [&values, &i] { return std::get<std::int64_t>(values[i++]); };
        auto boolean = [&number] { return number() != 0; };
        auto text = [&values, &i] { return std::get<std::string>(values[i++]); };

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        ip_address = text();
        user_agent = text();
        endpoint = text();
        method = static_cast<enums::HttpMethod>(number());
        action = text();
        entity_name = text();
        entity_id = number();
        parameters = text();
        request_body = text();
        status_code = number();
        error = text();
        success = boolean();
    }

    string AuthLog::validate()
    {
        using columns::AuthLogColumns;

        validator_chain_vector list{
            [this] { return test_at_least(user_id, 0, AuthLogColumns::USER_ID); },
            [this] { return testt_not_empty(endpoint, AuthLogColumns::ENDPOINT); },
            [this] { return test_ne(cast64(method), 0, AuthLogColumns::METHOD); },
            [this] { return test_true(status_code > 0, "Status code must be positive"); },
            [this] { return testt_at_most(error, 256, AuthLogColumns::ERROR); },
        };
        return util::ValidatorChain::run(list);
    }

    static std::string serializeParamsForAuth(const crow::query_string& qs)
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

    const AuthLog auth_log_from_crow_request(const crow::request& req, int user_id, int status_code,
                                             int entity_id, std::string error)
    {
        AuthLog log;

        log.user_id = user_id;
        log.ip_address = req.remote_ip_address;
        log.user_agent = req.get_header_value("User-Agent");
        log.endpoint = req.url; // optionally just path without query

        // method mapping
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


        // Entity name + ID - if it's part of the URL
        // e.g. /api/v1/note/123  → entity_name="note", entity_id=123
        {
            std::string path = req.url;
            // very simple parsing, regex or router metadata would be better in practice
            std::istringstream iss(path);
            std::string segment;
            std::vector<std::string> parts;
            while (std::getline(iss, segment, '/'))
            {
                if (!segment.empty()) parts.push_back(segment);
            }
            if (parts.size() >= 3 && parts[0] == "api" && parts[1].rfind("v", 0) == 0)
            {
                log.entity_name = parts[2];
                if (parts.size() >= 4)
                {
                    try { log.entity_id = std::stoi(parts[3]); }
                    catch (...) { log.entity_id = 0; }
                }
            }
            if (log.entity_id == 0 && entity_id != 0)
            {
                log.entity_id = entity_id;
            }
        }

        log.parameters = serializeParamsForAuth(req.url_params);
        log.request_body = req.body;
        log.status_code = status_code;
        log.error = error;
        log.success = (status_code >= 200 && status_code < 300);

        return log;
    }
}
