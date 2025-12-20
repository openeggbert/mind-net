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
#define Model TestAttemptAnswer
#define MODEL TEST_ATTEMPT_ANSWER
#define COLS columns::TestAttemptAnswerColumns
#include "../columns/TestAttemptAnswerColumns.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def TEST_ATTEMPT_ANSWER_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box #2", 100)
        .set_title_column(COLS::QUESTION_ID)
        .set_rest_operations("crl")
        .set_columns(
            {
                coldef(COLS::TEST_ATTEMPT_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::QUESTION_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::USER_ANSWER, TEXTAREA | READONLY),
                coldef(COLS::IS_CORRECT, BOOL | READONLY),
            });

    //todo: add unique index: test_attempt_id, question_id

    struct Model : mindnet::model::BaseModel
    {
        identification test_attempt_id{};
        identification question_id{};
        std::string user_answer;
        bool is_correct{};

        static constexpr auto fields = std::make_tuple(
            &Model::test_attempt_id,
            &Model::question_id,
            &Model::user_answer,
            &Model::is_correct
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id
                && created_at == other.created_at
                && updated_at == other.updated_at
                && test_attempt_id == other.test_attempt_id
                && question_id == other.question_id
                && user_answer == other.user_answer
                && is_correct == other.is_correct;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS