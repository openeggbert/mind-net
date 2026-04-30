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
#include "hive/model/BaseModel.hpp"
#include "hive/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model RefreshToken
#define MODEL REFRESH_TOKEN
#define COLS columns::RefreshTokenColumns
#include "../columns/RefreshTokenColumns.hpp"

// ***** MACROS : END *****

namespace hive::plugins::core::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def REFRESH_TOKEN_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("crl")
        .set_group("Core", 300)
        .set_title_column(COLS::TOKEN_HASH).allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::TOKEN_HASH, MANDATORY | UNIQUE),
            coldef(COLS::ISSUED_AT, MANDATORY | DATETIME),
            coldef(COLS::EXPIRES_AT, DATETIME),
            coldef(COLS::IS_REVOKED, MANDATORY | BOOL),
            coldef(COLS::REVOKED_AT, DATETIME),
            coldef(COLS::REPLACED_BY_ID).set_foreign_key("refresh_token"),
            coldef(COLS::ROTATED_FROM_ID).set_foreign_key("refresh_token"),
            coldef(COLS::LAST_USED_AT, DATETIME),
            coldef(COLS::IP_ADDRESS),
            coldef(COLS::USER_AGENT),
        });

    struct Model : hive::model::BaseModel
    {
        identification user_id{};
        string token_hash;
        unixtime issued_at{};
        unixtime expires_at{};
        bool is_revoked{false};
        unixtime revoked_at{};
        identification replaced_by_id{};
        identification rotated_from_id{};
        unixtime last_used_at{};
        string ip_address;
        string user_agent;

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::token_hash,
            &Model::issued_at,
            &Model::expires_at,
            &Model::is_revoked,
            &Model::revoked_at,
            &Model::replaced_by_id,
            &Model::rotated_from_id,
            &Model::last_used_at,
            &Model::ip_address,
            &Model::user_agent
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS