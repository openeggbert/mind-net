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
#include <utility>

#include "mindnet/model/BaseModel.hpp"
// ***** MACROS : START *****
#define Model Map
#define MODEL MAP
#define COLS columns::MapColumns
#include "../columns/MapColumns.hpp"
#include "mindnet/plugins/core/enums/AccessRight.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def MAP_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box", 100)
        .set_all_rest_operations().set_title_column(COLS::NAME)
        .set_columns({
            //
            coldef(COLS::NAME, MANDATORY | UNIQUE),
            coldef(COLS::DESCRIPTION),
            coldef(COLS::CATEGORY),
            coldef(COLS::OWNER_ID, MANDATORY).set_foreign_key("user"),
            coldef(COLS::TEAM_ID, FOREIGN_KEY),
            coldef(COLS::OWNER_RIGHTS, INTEGER | MANDATORY).set_default_value("7").set_enum_definition(
                core::enums::access_right_to_enum_definition()),
            coldef(COLS::TEAM_RIGHTS, INTEGER | MANDATORY).set_default_value("7").set_enum_definition(
                core::enums::access_right_to_enum_definition()),
            coldef(COLS::OTHER_RIGHTS, INTEGER | MANDATORY).set_default_value("7").set_enum_definition(
                core::enums::access_right_to_enum_definition()),
        })
        .add_custom_list_action("note", "List notes", {"map_id", "{id}"})
        .add_custom_create_action("note", "Add note", {"map_id", "{id}"})
        .add_custom_list_action("tag_type", "List tags", {"map_id", "{id}"})
        .add_custom_create_action("tag_type", "Add tag", {"map_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        string name;
        string description;
        string category;
        identification owner_id{};
        identification team_id{};
        core::enums::AccessRight owner_rights{7};
        core::enums::AccessRight team_rights{7};
        core::enums::AccessRight other_rights{7};

        static constexpr auto fields = std::make_tuple(
            &Model::name,
            &Model::description,
            &Model::category,
            &Model::owner_id,
            &Model::team_id,
            &Model::owner_rights,
            &Model::team_rights,
            &Model::other_rights
        );

        [[nodiscard]] int owner_rights_int() const { return cast64(owner_rights); }
        [[nodiscard]] int team_rights_int() const { return cast64(team_rights); }
        [[nodiscard]] int other_rights_int() const { return cast64(other_rights); }

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                name == other.name &&
                description == other.description &&
                category == other.category &&
                owner_id == other.owner_id &&
                team_id == other.team_id &&
                owner_rights == other.owner_rights &&
                team_rights == other.team_rights &&
                other_rights == other.other_rights;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS