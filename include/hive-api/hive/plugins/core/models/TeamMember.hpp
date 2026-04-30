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
#define Model TeamMember
#define MODEL TEAM_MEMBER
#define COLS columns::TeamMemberColumns
#include "../columns/TeamMemberColumns.hpp"
#include "../enums/UserStatus.hpp"
#include "hive/plugins/core/enums/UserRole.hpp"

// ***** MACROS : END *****

namespace hive::plugins::core::models
{
    using model::def;
    using model::coldef;
    using_flags();

    inline const def TEAM_MEMBER_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
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

    struct Model : hive::model::BaseModel
    {
        identification team_id{};
        identification user_id{};
        hive::essential::UserRole role{};
        hive::essential::UserStatus status{};
        unixtime joined_at{};
        unixtime left_at{};

        static constexpr auto fields = std::make_tuple(
            &Model::team_id,
            &Model::user_id,
            &Model::role,
            &Model::status,
            &Model::joined_at,
            &Model::left_at
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS