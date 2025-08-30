///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#include "mindnet/models/misc/BaseModel.h"

// ***** MACROS : START *****
#define Model User
#define MODEL USER
#include "columns/UserColumns.h"
// ***** MACROS : END *****
#include "mindnet/enums/UserRole.h"
#include "mindnet/enums/UserStatus.h"

namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::UserColumns;
    using misc::def;
    using misc::coldef;

    inline def USER_DEFINITION =
        def(cols::MODEL_NAME)
        .set_rest_operations("crudl")
        .set_columns({
            //
            coldef(cols::USERNAME).set_mandatory().set_unique(),
            coldef(cols::PASSWORD_HASH).set_mandatory(),
            coldef(cols::DISPLAY_NAME),
            coldef(cols::ROLE).set_mandatory().set_enum_definition(enums::user_role_to_enum_definition()).
                               set_default_value("0"),
            coldef(cols::PROFILE_TEXT).textarea(),
            coldef(cols::LAST_LOGIN).datetime(),
            coldef(cols::EMAIL).set_unique(),
            coldef(cols::STATUS).set_mandatory().set_enum_definition(enums::user_status_to_enum_definition()),
        });

    struct Model : bm
    {
        Model() = default;


        str username;
        str password_hash;
        str display_name;
        int role{};
        str profile_text;
        unixtime last_login{};
        str email;
        int status{};

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

#endif // USER_H
