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
#define Model User
#define MODEL USER
#define COLS columns::UserColumns
#include "../columns/UserColumns.hpp"
// ***** MACROS : END *****
#include "../enums/UserStatus.hpp"
#include "mindnet/plugins/core/enums/UserRole.hpp"

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def USER_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("crul")
        .set_group("Core", 300)
        .set_title_column(COLS::USERNAME).allow_reader_write()
        .set_columns({
            //
            coldef(COLS::USERNAME, MANDATORY | UNIQUE | READONLY),
            coldef(COLS::PASSWORD_HASH, MANDATORY | HIDDEN),
            coldef(COLS::EMAIL, UNIQUE | READONLY),

            coldef(COLS::DISPLAY_NAME),
            coldef(COLS::PROFILE_TEXT, TEXTAREA),

            coldef(COLS::ROLE, MANDATORY).set_enum_definition(enums::user_role_to_enum_definition()).
                                          set_default_value(0),
            coldef(COLS::STATUS, MANDATORY).set_enum_definition(enums::user_status_to_enum_definition()),

            coldef(COLS::LAST_LOGIN, DATETIME),
        });

    struct Model : mindnet::model::BaseModel
    {
        string username;
        string password_hash;
        string email;

        string display_name;
        string profile_text;

        mindnet::essential::UserRole role{};
        essential::UserStatus status{};

        unixtime last_login{};

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
