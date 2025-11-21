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
#define Model TestAttempt
#define MODEL TEST_ATTEMPT
#define COLS columns::TestAttemptColumns
#include "../columns/TestAttemptColumns.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def TEST_ATTEMPT_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box", 100)
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
            });

    struct Model : mindnet::model::BaseModel
    {
        i64 test_id{};
        i64 user_id{};
        int attempt_number{};
        unixtime started_at;
        unixtime finished_at;
        int score_times_100{};
        std::string question_ids;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id
                && created_at == other.created_at
                && updated_at == other.updated_at
                && test_id == other.test_id
                && user_id == other.user_id
                && attempt_number == other.attempt_number
                && started_at == other.started_at
                && finished_at == other.finished_at
                && score_times_100 == other.score_times_100
                && question_ids == other.question_ids;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
