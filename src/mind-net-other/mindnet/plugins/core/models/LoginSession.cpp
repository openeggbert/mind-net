//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/core/models/LoginSession.h"
#include "mindnet/plugins/core/columns/LoginSessionColumns.h"

namespace mindnet::plugins::core::models
{
    entity_fields LoginSession::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        //
        result.push_back(user_id);
        result.push_back(access_token_id);
        result.push_back(refresh_token_id);
        result.push_back(cast64(issued_at));
        result.push_back(cast64(expires_at));
        result.push_back(is_revoked);
        result.push_back(cast64(revoked_at));
        result.push_back(cast64(last_used_at));
        result.push_back(ip_address);
        result.push_back(user_agent);
        return result;
    }

    void LoginSession::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        user_id = number();
        access_token_id = number();
        refresh_token_id = number();
        issued_at = number();
        expires_at = number();
        is_revoked = boolean();
        revoked_at = number();
        last_used_at = number();
        ip_address = text();
        user_agent = text();
    }

    string LoginSession::validate()
    {
        using columns::LoginSessionColumns;

        validator_chain_vector list{
            [this] { return test_not_null(user_id, LoginSessionColumns::USER_ID); },
            [this] { return test_not_null(access_token_id, LoginSessionColumns::ACCESS_TOKEN_ID); },
            [this] { return test_not_null(issued_at, LoginSessionColumns::ISSUED_AT); },
            [this]
            {
                return test_true(expires_at == 0 || expires_at > issued_at,
                                 "Expiration time must be after issued time");
            },
            [this]
            {
                return test_true(!is_revoked || revoked_at > 0,
                                 "Revoked session must have revocation time");
            }
        };

        return util::ValidatorChain::run(list);
    }
}
