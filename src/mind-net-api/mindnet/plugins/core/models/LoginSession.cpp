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

#include "mindnet/plugins/core/models/LoginSession.hpp"
#include "mindnet/plugins/core/columns/LoginSessionColumns.hpp"

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
