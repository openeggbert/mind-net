///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
// or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ACCESS_TOKEN_H
#define ACCESS_TOKEN_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model AccessToken
#define MODEL ACCESS_TOKEN
#define COLS columns::AccessTokenColumns
#include "../columns/AccessTokenColumns.h"
#include "mindnet/plugins/core/enums/TokenPurpose.h"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def ACCESS_TOKEN_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("crl")
        .set_group("Core", 300)
        .set_title_column(COLS::TOKEN_HASH).allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::TOKEN_HASH, MANDATORY | UNIQUE),
            coldef(COLS::TOKEN_PURPOSE, MANDATORY).set_enum_definition(enums::token_purpose_to_enum_definition()),
            coldef(COLS::IS_CREATED_BY_USER, MANDATORY),
            coldef(COLS::NAME),
            coldef(COLS::DESCRIPTION, TEXTAREA),
            coldef(COLS::ISSUED_AT, MANDATORY | DATETIME),
            coldef(COLS::EXPIRES_AT, DATETIME),
            coldef(COLS::IS_REVOKED, MANDATORY),
            coldef(COLS::REVOKED_AT, DATETIME),
            coldef(COLS::LAST_USED_AT, DATETIME),
            coldef(COLS::IP_ADDRESS),
            coldef(COLS::USER_AGENT),
            coldef(COLS::ALLOWED_OPERATIONS),
        });

    struct Model : mindnet::model::BaseModel
    {
        int64_t user_id{};
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
#endif // ACCESS_TOKEN_H
