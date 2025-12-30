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
#include "mindnet/plugins/repetition/RepetitionPlugin.hpp"

// ***** MACROS : START *****
#define Model RSession
#define MODEL R_SESSION
#define COLS columns::RSessionColumns
#include "../columns/RSessionColumns.hpp"
#include "mindnet/plugins/repetition/enums/RepetitionAlgorithm.hpp"
#include "mindnet/plugins/repetition/enums/RepetitionSchedule.hpp"
#include "mindnet/plugins/repetition/enums/RepetitionScope.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R_SESSION_DEFINITION =
        def(COLS::MODEL_NAME, REPETITION_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Repetition", 200)
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::MAP_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::MAP_COLLECTION_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::CLONED_FROM_SESSION_ID, READONLY).set_foreign_key("r_session"),

            coldef(COLS::ALGORITHM, MANDATORY | READONLY).set_enum_definition(
                enums::repetition_algorithm_to_enum_definition()),
            coldef(COLS::SCHEDULE, MANDATORY | READONLY).set_enum_definition(
                enums::repetition_schedule_to_enum_definition()),

            coldef(COLS::SCOPE, MANDATORY | READONLY).set_enum_definition(enums::repetition_scope_to_enum_definition()),
            coldef(COLS::DESCRIPTION),

            coldef(COLS::FILTER_ELIGIBLE, BOOL | READONLY).set_default_value(false),
            coldef(COLS::FILTER_UNDER_NOTE, READONLY).set_foreign_key("note"),
            coldef(COLS::FILTER_DATE_FROM, DATETIME | READONLY),
            coldef(COLS::FILTER_DATE_TO, DATETIME | READONLY),
            coldef(COLS::FILTER_TAG, READONLY).set_foreign_key("tag"),
            coldef(COLS::FILTER_COLLECTION, READONLY).set_foreign_key("collection"),

            coldef(COLS::SELECTED_ITEMS, MANDATORY).set_default_value("{}"),
            coldef(COLS::PINNED, BOOL).set_default_value(false)
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        identification map_id{};
        identification map_collection_id{};
        identification cloned_from_session_id{};
        enums::RepetitionAlgorithm algorithm{};
        enums::RepetitionSchedule schedule{};
        enums::RepetitionScope scope{};
        std::string description;
        bool filter_eligible{false};
        identification filter_under_note{};
        unixtime filter_date_from{};
        unixtime filter_date_to{};
        identification filter_tag{};
        identification filter_collection{};
        //example: {"note_ids":[3,4,5,6,7]}
        string selected_items{"{}"};
        bool pinned{false};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::map_id,
            &Model::map_collection_id,
            &Model::cloned_from_session_id,
            &Model::algorithm,
            &Model::schedule,
            &Model::scope,
            &Model::description,
            &Model::filter_eligible,
            &Model::filter_under_note,
            &Model::filter_date_from,
            &Model::filter_date_to,
            &Model::filter_tag,
            &Model::filter_collection,
            &Model::selected_items,
            &Model::pinned
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS