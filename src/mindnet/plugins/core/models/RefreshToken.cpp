#include "mindnet/plugins/core/models/RefreshToken.h"
#include "mindnet/plugins/core/columns/RefreshTokenColumns.h"

namespace mindnet::plugins::core::models
{
    entity_fields RefreshToken::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        //
        result.push_back(cast64(user_id));
        result.push_back(token_hash);
        result.push_back(cast64(issued_at));
        result.push_back(cast64(expires_at));
        result.push_back(cast64(is_revoked));
        result.push_back(cast64(revoked_at));
        result.push_back(cast64(replaced_by_id));
        result.push_back(cast64(rotated_from_id));
        result.push_back(cast64(last_used_at));
        result.push_back(ip_address);
        result.push_back(user_agent);
        return result;
    }

    void RefreshToken::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        user_id = number();
        token_hash = text();
        issued_at = number();
        expires_at = number();
        is_revoked = number();
        revoked_at = number();
        replaced_by_id = number();
        rotated_from_id = number();
        last_used_at = number();
        ip_address = text();
        user_agent = text();
    }

    string RefreshToken::validate()
    {
        using columns::RefreshTokenColumns;

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0, RefreshTokenColumns::USER_ID); },
            [this] { return testt_not_empty(token_hash, RefreshTokenColumns::TOKEN_HASH); },
            [this] { return test_ne(issued_at, 0, RefreshTokenColumns::ISSUED_AT); }
        };

        return ValidatorChain::run(list);
    }
}
