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

#include "hive/model/BaseModel.hpp"
// ***** MACROS : START *****
#define Model DictionaryMap
#define MODEL DICTIONARY_MAP
#define COLS columns::DictionaryMapColumns
#include "../columns/DictionaryMapColumns.hpp"
#include "hive/plugins/core/enums/AccessRight.hpp"
#include "hive/plugins/dictionary/DictionaryPlugin.hpp"

// ***** MACROS : END *****

namespace hive::plugins::dictionary::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def DICTIONARY_MAP_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_group("Dictionary", 100)
        .set_all_rest_operations().set_title_column(COLS::NAME)
        .set_columns({
            //
            coldef(COLS::NAME, MANDATORY | UNIQUE),
            coldef(COLS::DESCRIPTION),
            coldef(COLS::POSITION, INTEGER).set_default_value(0),
            coldef(COLS::EMOJI).set_default_value("📘"),
            coldef(COLS::OWNER_ID, MANDATORY).set_foreign_key("user"),
            coldef(COLS::TEAM_ID, FOREIGN_KEY),
            coldef(COLS::OWNER_RIGHTS, INTEGER | MANDATORY).set_default_value("7").set_enum_definition(
                core::enums::access_right_to_enum_definition()),
            coldef(COLS::TEAM_RIGHTS, INTEGER | MANDATORY).set_default_value("7").set_enum_definition(
                core::enums::access_right_to_enum_definition()),
            coldef(COLS::OTHER_RIGHTS, INTEGER | MANDATORY).set_default_value("7").set_enum_definition(
                core::enums::access_right_to_enum_definition()),
        })
        .add_custom_list_action("dictionary_term", "List terms", {"dictionary_map_id", "{id}"})
        .add_custom_create_action("dictionary_term", "Add term", {"dictionary_map_id", "{id}"})
        .add_custom_list_action("dictionary_tag_type", "List tags", {"dictionary_map_id", "{id}"})
        .add_custom_create_action("dictionary_tag_type", "Add tag", {"dictionary_map_id", "{id}"});

    struct Model : hive::model::BaseModel
    {
        string name;
        string description;
        int position{0};
        string emoji;
        identification owner_id{};
        identification team_id{};
        core::enums::AccessRight owner_rights{7};
        core::enums::AccessRight team_rights{7};
        core::enums::AccessRight other_rights{7};
        [[nodiscard]] int owner_rights_int() const { return cast64(owner_rights); }
        [[nodiscard]] int team_rights_int() const { return cast64(team_rights); }
        [[nodiscard]] int other_rights_int() const { return cast64(other_rights); }

        static constexpr auto fields = std::make_tuple(
    &Model::name,
    &Model::description,
    &Model::position,
    &Model::emoji,
    &Model::owner_id,
    &Model::team_id,
    &Model::owner_rights,
    &Model::team_rights,
    &Model::other_rights
        );
        
        create_model_h_methods(Model, MODEL)
    };
}
#undef Model
#undef MODEL
#undef COLS
