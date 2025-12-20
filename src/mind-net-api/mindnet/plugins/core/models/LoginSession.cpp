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
    create_model_cpp_methods(LoginSession)

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