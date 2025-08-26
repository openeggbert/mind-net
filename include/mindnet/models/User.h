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

// ***** DEFINE SECTION : START *****
#define Model User
#define MODEL USER
// ***** DEFINE SECTION : END *****
#include "columns/UserColumns.h"

namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::UserColumns;
    using misc::def;
    using misc::coldef;

    inline def USER_DEFINITION = def(cols::MODEL_NAME)

        .set_columns({
            //
            coldef(cols::USERNAME).set_mandatory(true).set_unique(true),
            coldef(cols::PASSWORD_HASH).set_mandatory(true),
            coldef(cols::DISPLAY_NAME),
            coldef(cols::ROLE).set_mandatory(true), // enum() TODO
            coldef(cols::PROFILE_TEXT).textarea(),
            coldef(cols::LAST_LOGIN).datetime(),
            coldef(cols::EMAIL).set_unique(true),
            coldef(cols::STATUS).set_mandatory(true),
        })
    ;

    struct User : bm
    {
        User() = default;

        // ***** DEFINE FIELDS : START *****
        str table_name;
        int record_id{};
        enums::Crudl operation{};
        str data_json;
        str reason;
        // ***** DEFINE FIELDS : END *****

        create_model_h_methods(Model, MODEL)

        // ***** Implement methods operator== : START *****
        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                table_name == other.table_name &&
                record_id == other.record_id &&
                operation == other.operation &&
                data_json == other.data_json &&
                reason == other.reason;
        }

        // ***** Implement methods operator== : END *****
    };
}
#undef Model
#undef MODEL

#endif // USER_H
