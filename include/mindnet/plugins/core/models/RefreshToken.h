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
#ifndef REFRESH_TOKEN_H
#define REFRESH_TOKEN_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model RefreshToken
#define MODEL REFRESH_TOKEN
#define COLS columns::RefreshTokenColumns
#include "../columns/RefreshTokenColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def REFRESH_TOKEN_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("crl")
        .set_group("Core", 300)
        .set_title_column(COLS::TOKEN_HASH).allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::TOKEN_HASH, MANDATORY | UNIQUE),
            coldef(COLS::ISSUED_AT, MANDATORY | DATETIME),
            coldef(COLS::EXPIRES_AT, DATETIME),
            coldef(COLS::IS_REVOKED, MANDATORY),
            coldef(COLS::REVOKED_AT, DATETIME),
            coldef(COLS::REPLACED_BY_ID).set_foreign_key("refresh_token"),
            coldef(COLS::ROTATED_FROM_ID).set_foreign_key("refresh_token"),
            coldef(COLS::LAST_USED_AT, DATETIME),
            coldef(COLS::IP_ADDRESS),
            coldef(COLS::USER_AGENT),
        });

    struct Model : mindnet::model::BaseModel
    {
        int64_t user_id{};
        string token_hash;
        unixtime issued_at{};
        unixtime expires_at{};
        bool is_revoked{false};
        unixtime revoked_at{};
        int64_t replaced_by_id{};
        int64_t rotated_from_id{};
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
                token_hash == other.token_hash &&
                issued_at == other.issued_at &&
                expires_at == other.expires_at &&
                is_revoked == other.is_revoked &&
                revoked_at == other.revoked_at &&
                replaced_by_id == other.replaced_by_id &&
                rotated_from_id == other.rotated_from_id &&
                last_used_at == other.last_used_at &&
                ip_address == other.ip_address &&
                user_agent == other.user_agent;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // REFRESH_TOKEN_H
