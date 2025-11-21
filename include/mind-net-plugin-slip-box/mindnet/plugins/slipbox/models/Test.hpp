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
#pragma once

#include <string>

#include "mindnet/model/BaseModel.hpp"
// ***** MACROS : START *****
#define Model Test
#define MODEL TEST
#define COLS columns::TestColumns
#include "../columns/TestColumns.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    constexpr int SECONDS_PER_DAY = 86400;
    inline const def TEST_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box #2", 100)
        .set_title_column(COLS::TITLE)
        .set_columns(
            {
                coldef(COLS::UNDER_NOTE_ID, MANDATORY | FOREIGN_KEY | READONLY).set_foreign_key("note"),
                coldef(COLS::MAP_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::TITLE, MANDATORY),
                coldef(COLS::DESCRIPTION),
                coldef(COLS::TIME_LIMIT_IN_SECONDS, INTEGER).set_default_value(SECONDS_PER_DAY),
                coldef(COLS::ANSWER_COUNT_LIMIT, INTEGER).set_default_value(100),
                coldef(COLS::ATTEMPT_LIMIT, INTEGER).set_default_value(1),
                coldef(COLS::IS_PUBLIC, BOOL | READONLY).set_default_value(true),
            })
    .add_custom_list_action("test_attempt", "List test attempts", {"test_id", "{id}"})
    .add_custom_create_action("test_attempt", "Create test attempt", {"test_id", "{id}"})
    ;

    struct Model : mindnet::model::BaseModel
    {
        i64 under_note_id{};
        i64 map_id{};
        std::string title;
        std::string description;
        int time_limit_in_seconds{};
        int answer_count_limit{};
        int attempt_limit{};
        bool is_public{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id
                && created_at == other.created_at
                && updated_at == other.updated_at
                && map_id == other.map_id
                && under_note_id == other.under_note_id
                && title == other.title
                && description == other.description
                && time_limit_in_seconds == other.time_limit_in_seconds
                && answer_count_limit == other.answer_count_limit
                && attempt_limit == other.attempt_limit
                && is_public == other.is_public;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
