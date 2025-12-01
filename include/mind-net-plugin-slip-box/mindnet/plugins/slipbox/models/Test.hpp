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
                coldef(COLS::UNDER_NOTE_ID, FOREIGN_KEY | READONLY).set_foreign_key("note"),
                coldef(COLS::MAP_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::TITLE, MANDATORY),
                coldef(COLS::DESCRIPTION),
                coldef(COLS::TIME_LIMIT_IN_SECONDS, INTEGER).set_default_value(SECONDS_PER_DAY),
                coldef(COLS::ANSWER_COUNT_LIMIT, INTEGER).set_default_value(100),
                coldef(COLS::ATTEMPT_LIMIT, INTEGER).set_default_value(1),
                coldef(COLS::IS_PUBLIC, BOOL | READONLY).set_default_value(true),
            })
        .add_custom_list_action("test_attempt", "List test attempts", {"test_id", "{id}"})
        .add_custom_create_action("test_attempt", "Create test attempt", {"test_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        identification under_note_id{};
        identification map_id{};
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