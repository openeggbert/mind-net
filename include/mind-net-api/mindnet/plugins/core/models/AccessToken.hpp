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

// ***** MACROS : START *****
#define Model AccessToken
#define MODEL ACCESS_TOKEN
#define COLS columns::AccessTokenColumns
#include "../columns/AccessTokenColumns.hpp"
#include "mindnet/plugins/core/CorePlugin.hpp"
#include "mindnet/plugins/core/enums/TokenPurpose.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def ACCESS_TOKEN_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("crl")
        .set_group("Core", 300)
        .set_title_column(COLS::TOKEN_HASH).allow_reader_write().set_cache_enabled(false)
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::TOKEN_HASH, MANDATORY | UNIQUE),
            coldef(COLS::TOKEN_PURPOSE, MANDATORY).set_enum_definition(enums::token_purpose_to_enum_definition()),
            coldef(COLS::IS_CREATED_BY_USER, BOOL | MANDATORY),
            coldef(COLS::NAME),
            coldef(COLS::DESCRIPTION, TEXTAREA),
            coldef(COLS::ISSUED_AT, MANDATORY | DATETIME),
            coldef(COLS::EXPIRES_AT, DATETIME),
            coldef(COLS::IS_REVOKED, BOOL | MANDATORY),
            coldef(COLS::REVOKED_AT, DATETIME),
            coldef(COLS::LAST_USED_AT, DATETIME),
            coldef(COLS::IP_ADDRESS),
            coldef(COLS::USER_AGENT),
            coldef(COLS::ALLOWED_OPERATIONS),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        string token_hash;
        enums::TokenPurpose token_purpose{};
        bool is_created_by_user{false};
        string name;
        string description;
        unixtime issued_at{};
        unixtime expires_at{};
        bool is_revoked{false};
        unixtime revoked_at{};
        unixtime last_used_at{};
        string ip_address;
        string user_agent;

        /**
 * Column: allowed_operations (TEXT)
 *
 * Defines CRUDL permissions for the access token.
 *
 * Syntax:
 *   - "*"                  → all operations on all entities
 *   - "rl"                 → read + list on all entities
 *   - "note=crudl"         → full access (create, read, update, delete, list) on the "note" entity
 *   - "user=rl"            → read + list on the "user" entity
 *   - "rl note=crud"       → default read + list everywhere, but full CRUD on "note"
 *   - "note:123=ru"        → entity-instance specific rights (read + update on note with id=123)
 *
 * Operation flags:
 *   c = create
 *   r = read
 *   u = update
 *   d = delete
 *   l = list
 *
 * Multiple rules are separated by spaces. If both global and entity-specific rules
 * exist, the entity-specific rule overrides the global one.
 *
 * Examples:
 *   "*"                        → superuser token
 *   "rl"                       → read-only access on all entities
 *   "note=crul user=rl"        → full rights on notes, read/list only on users
 *   "rl project=crud task=rl"  → read/list globally, but full CRUD on projects,
 *                                 read/list on tasks
 */

        string allowed_operations;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                token_hash == other.token_hash &&
                token_purpose == other.token_purpose &&
                is_created_by_user == other.is_created_by_user &&
                name == other.name &&
                description == other.description &&
                issued_at == other.issued_at &&
                expires_at == other.expires_at &&
                is_revoked == other.is_revoked &&
                revoked_at == other.revoked_at &&
                last_used_at == other.last_used_at &&
                ip_address == other.ip_address &&
                user_agent == other.user_agent &&
                allowed_operations == other.allowed_operations;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS