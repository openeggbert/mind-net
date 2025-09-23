///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef USER_H
#define USER_H

#include <string>
#include "../../../../../mind-net-model/mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model User
#define MODEL USER
#define COLS columns::UserColumns
#include "../columns/UserColumns.h"
// ***** MACROS : END *****
#include "../enums/UserStatus.h"
#include "mindnet/plugins/core/enums/UserRole.h"

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def USER_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("crl")
        .set_group("Core", 300)
        .set_title_column(COLS::USERNAME).allow_reader_write()
        .set_columns({
            //
            coldef(COLS::USERNAME, MANDATORY | UNIQUE | READONLY),
            coldef(COLS::PASSWORD_HASH, MANDATORY | HIDDEN),
            coldef(COLS::DISPLAY_NAME),
            coldef(COLS::ROLE, MANDATORY).set_enum_definition(enums::user_role_to_enum_definition()).
                                          set_default_value(0),
            coldef(COLS::PROFILE_TEXT, TEXTAREA),
            coldef(COLS::LAST_LOGIN, DATETIME),
            coldef(COLS::EMAIL, UNIQUE | READONLY),
            coldef(COLS::STATUS, MANDATORY).set_enum_definition(enums::user_status_to_enum_definition()),
        });

    struct Model : mindnet::model::BaseModel
    {
        string username;
        string password_hash;
        string display_name;
        mindnet::core::UserRole role{};
        string profile_text;
        unixtime last_login{};
        string email;
        enums::UserStatus status{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                username == other.username &&
                password_hash == other.password_hash &&
                display_name == other.display_name &&
                role == other.role &&
                profile_text == other.profile_text &&
                last_login == other.last_login &&
                email == other.email &&
                status == other.status;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // USER_H
