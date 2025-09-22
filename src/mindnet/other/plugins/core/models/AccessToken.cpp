//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/core/models/AccessToken.h"

namespace mindnet::plugins::core::models
{
    entity_fields AccessToken::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        //
        result.push_back(user_id);
        result.push_back(token_hash);
        result.push_back(cast64(token_purpose));
        result.push_back(cast64(is_created_by_user));
        result.push_back(name);
        result.push_back(description);
        result.push_back(cast64(issued_at));
        result.push_back(cast64(expires_at));
        result.push_back(cast64(is_revoked));
        result.push_back(cast64(revoked_at));
        result.push_back(cast64(last_used_at));
        result.push_back(ip_address);
        result.push_back(user_agent);
        result.push_back(allowed_operations);
        return result;
    }

    void AccessToken::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        user_id = number();
        token_hash = text();
        token_purpose = static_cast<enums::TokenPurpose>(number());
        is_created_by_user = static_cast<bool>(number());
        name = text();
        description = text();
        issued_at = number();
        expires_at = number();
        is_revoked = static_cast<bool>(number());
        revoked_at = number();
        last_used_at = number();
        ip_address = text();
        user_agent = text();
        allowed_operations = text();
    }

    string AccessToken::validate()
    {
        using columns::AccessTokenColumns;

        using_test_utils()

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0,AccessTokenColumns::USER_ID); },
            [this] { return testt_not_empty(token_hash, AccessTokenColumns::TOKEN_HASH); },
            [this] { return test_ne(issued_at, 0, AccessTokenColumns::ISSUED_AT); },
            [this]
            {
                if (is_created_by_user)
                {
                    return testt_not_empty(name, AccessTokenColumns::NAME);
                }
                return util::test_result{};
            },
            [this]
            {
                if (!name.empty())
                {
                    return testt_at_most(name, 64, AccessTokenColumns::NAME);
                }
                return util::test_result{};
            },
            [this]
            {
                if (!description.empty())
                {
                    return testt_at_most(description, 256, AccessTokenColumns::DESCRIPTION);
                }
                return util::test_result{};
            }
        };

        return util::ValidatorChain::run(list);
    }
}
