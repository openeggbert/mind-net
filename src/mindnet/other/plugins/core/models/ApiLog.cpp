//
// Created by robertvokac on 9/21/25.
//

#include "mindnet/other/plugins/core/models/ApiLog.h"

namespace mindnet::plugins::core::models
{
    entity_fields ApiLog::to_values() const
    {
        entity_fields result;
        result.emplace_back(id);
        result.emplace_back(cast64(created_at));
        result.emplace_back(cast64(updated_at));
        result.emplace_back(cast64(user_id));
        result.emplace_back(ip_address);
        result.emplace_back(endpoint);
        result.emplace_back(cast64(method)); // enum HttpMethod stored as number
        result.emplace_back(entity_name);
        result.emplace_back(cast64(entity_id));
        result.emplace_back(parameters);
        result.emplace_back(request_body);
        result.emplace_back(cast64(status_code));
        result.emplace_back(error);
        return result;
    }

    void ApiLog::from_values(const entity_fields& values)
    {
        int i = 0;

        auto number = [&values, &i] { return std::get<std::int64_t>(values[i++]); };
        auto boolean = [&number] { return number() != 0; };
        auto text = [&values, &i] { return std::get<std::string>(values[i++]); };

        set_id(number());
        created_at   = number();
        updated_at   = number();
        user_id      = number();
        ip_address   = text();
        endpoint     = text();
        method       = static_cast<enums::HttpMethod>(number());
        entity_name  = text();
        entity_id    = number();
        parameters   = text();
        request_body = text();
        status_code  = number();
        error         = text();
    }

    string ApiLog::validate()
    {
        using columns::ApiLogColumns;

        validator_chain_vector list{
            [this] { return test_at_least(user_id, 0, ApiLogColumns::USER_ID); },
            [this] { return testt_not_empty(endpoint, ApiLogColumns::ENDPOINT); },
            [this] { return test_ne(cast64(method), 0, ApiLogColumns::METHOD); },
            [this] { return testt_not_empty(entity_name, ApiLogColumns::ENTITY_NAME); },
            [this] { return test_true(status_code > 0, "Status code must be positive"); },
            [this] { return testt_at_most(error, 256, ApiLogColumns::ERROR); },
        };
        return util::ValidatorChain::run(list);
    }
}
