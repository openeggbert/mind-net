///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend written in modern C++.
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
        .set_group("Slip Box", 100)
        .set_title_column(COLS::QUESTION_ID)
        .set_rest_operations("crl")
        .set_columns(
            {
                coldef(COLS::TEST_ATTEMPT_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::QUESTION_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::USER_ANSWER, TEXTAREA | READONLY),
                coldef(COLS::IS_CORRECT, BOOL | READONLY),
            });

    struct Model : mindnet::model::BaseModel
    {
        i64 test_attempt_id{};
        i64 question_id{};
        std::string user_answer;
        bool is_correct{};

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
