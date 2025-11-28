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

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#pragma once

#include "mindnet/model/BaseColumns.hpp"

namespace mindnet::plugins::core::columns
{
    struct LoginSessionColumns : model::BaseColumns
    {
        LoginSessionColumns() = delete;
        LoginSessionColumns(const LoginSessionColumns&) = delete;
        LoginSessionColumns& operator=(const LoginSessionColumns&) = delete;

        static constexpr const char* MODEL_NAME = "login_session";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* ACCESS_TOKEN_ID = "access_token_id";
        static constexpr const char* REFRESH_TOKEN_ID = "refresh_token_id";
        static constexpr const char* ISSUED_AT = "issued_at";
        static constexpr const char* EXPIRES_AT = "expires_at";
        static constexpr const char* IS_REVOKED = "is_revoked";
        static constexpr const char* REVOKED_AT = "revoked_at";
        static constexpr const char* LAST_USED_AT = "last_used_at";
        static constexpr const char* IP_ADDRESS = "ip_address";
        static constexpr const char* USER_AGENT = "user_agent";
    };
}
