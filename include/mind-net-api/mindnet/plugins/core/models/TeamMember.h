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
#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model TeamMember
#define MODEL TEAM_MEMBER
#define COLS columns::TeamMemberColumns
#include "../columns/TeamMemberColumns.h"
#include "../enums/UserStatus.h"
#include "mindnet/plugins/core/enums/UserRole.h"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using model::def;
    using model::coldef;
    using_flags();

    inline def TEAM_MEMBER_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("rl")
        .set_group("Core", 300)
        .set_columns({
            //
            coldef(COLS::TEAM_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::ROLE, MANDATORY).set_default_value(0).set_enum_definition(
                enums::user_role_to_enum_definition()),
            coldef(COLS::STATUS, MANDATORY).set_enum_definition(enums::user_status_to_enum_definition()),
            coldef(COLS::JOINED_AT, DATETIME | MANDATORY | READONLY),
            coldef(COLS::LEFT_AT, DATETIME),

        });

    struct Model : mindnet::model::BaseModel
    {
        int team_id{};
        int user_id{};
        mindnet::essential::UserRole role{};
        mindnet::essential::UserStatus status{};
        unixtime joined_at{};
        unixtime left_at{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                team_id == other.team_id &&
                user_id == other.user_id &&
                role == other.role &&
                status == other.status &&
                joined_at == other.joined_at &&
                left_at == other.left_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // TEAMMEMBER_H
