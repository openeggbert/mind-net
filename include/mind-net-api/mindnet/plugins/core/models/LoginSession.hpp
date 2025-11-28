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

#pragma once


#include <string>
#include "mindnet/model/BaseModel.hpp"
#include "mindnet/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model LoginSession
#define MODEL LOGIN_SESSION
#define COLS columns::LoginSessionColumns
#include "../columns/LoginSessionColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def LOGIN_SESSION_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Core", 300)
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::ACCESS_TOKEN_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::REFRESH_TOKEN_ID, FOREIGN_KEY),
            coldef(COLS::ISSUED_AT, MANDATORY | DATETIME),
            coldef(COLS::EXPIRES_AT, DATETIME),
            coldef(COLS::IS_REVOKED, MANDATORY | BOOL),
            coldef(COLS::REVOKED_AT, DATETIME),
            coldef(COLS::LAST_USED_AT, DATETIME),
            coldef(COLS::IP_ADDRESS),
            coldef(COLS::USER_AGENT),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        identification access_token_id{};
        identification refresh_token_id{};
        unixtime issued_at{};
        unixtime expires_at{};
        bool is_revoked{false};
        unixtime revoked_at{};
        unixtime last_used_at{};
        string ip_address;
        string user_agent;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                access_token_id == other.access_token_id &&
                refresh_token_id == other.refresh_token_id &&
                issued_at == other.issued_at &&
                expires_at == other.expires_at &&
                is_revoked == other.is_revoked &&
                revoked_at == other.revoked_at &&
                last_used_at == other.last_used_at &&
                ip_address == other.ip_address &&
                user_agent == other.user_agent;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
