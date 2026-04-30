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
// ***** MACROS : START *****
#define Model TestAttempt
#define MODEL TEST_ATTEMPT
#define COLS columns::TestAttemptColumns
#include "../columns/TestAttemptColumns.hpp"
#include "hive/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def TEST_ATTEMPT_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box #2", 1000)
        .set_title_column(COLS::ATTEMPT_NUMBER)
        .set_rest_operations("crl")
        .set_columns(
            {
                coldef(COLS::TEST_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::ATTEMPT_NUMBER, INTEGER | MANDATORY | READONLY),
                coldef(COLS::STARTED_AT, DATETIME).set_default_value(0),
                coldef(COLS::FINISHED_AT, DATETIME).set_default_value(0),
                coldef(COLS::SCORE_TIMES_100, INTEGER),
                coldef(COLS::QUESTION_IDS),
            })
        .add_custom_list_action("test_attempt_answer", "List answers", {"test_attempt_id", "{id}"});

    struct Model : hive::model::BaseModel
    {
        identification test_id{};
        identification user_id{};
        int attempt_number{};
        unixtime started_at{};
        unixtime finished_at{};
        int score_times_100{};
        std::string question_ids;

        static constexpr auto fields = std::make_tuple(
            &Model::test_id,
            &Model::user_id,
            &Model::attempt_number,
            &Model::started_at,
            &Model::finished_at,
            &Model::score_times_100,
            &Model::question_ids
        );

        create_model_h_methods(Model, MODEL)

    };
}

#undef Model
#undef MODEL
#undef COLS