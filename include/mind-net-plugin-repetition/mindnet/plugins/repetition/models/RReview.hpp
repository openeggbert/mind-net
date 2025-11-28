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
#define Model RReview
#define MODEL R_REVIEW
#define COLS columns::RReviewColumns
#include "../columns/RReviewColumns.hpp"
#include "mindnet/plugins/repetition/enums/RepetitionAlgorithm.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R_REVIEW_DEFINITION =
        def(COLS::MODEL_NAME, REPETITION_PLUGIN_NAME)
        .set_rest_operations("crl")
        .set_group("Repetition", 200).set_title_column(COLS::NOTE_ID).allow_reader_write()
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::MAP_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::R_SESSION_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::ALGORITHM, MANDATORY | READONLY).set_enum_definition(
                enums::repetition_algorithm_to_enum_definition()),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::REVIEW_DATE, DATETIME | READONLY),
            coldef(COLS::GRADE, INTEGER | READONLY),
            coldef(COLS::RESPONSE_DATA, READONLY),
            coldef(COLS::NOTES, READONLY),
            coldef(COLS::STARTED_AT, DATETIME | READONLY),
            coldef(COLS::ENDED_AT, DATETIME | READONLY),
            coldef(COLS::LATENCY_MS, INTEGER | READONLY),
            coldef(COLS::ANSWER_CHANGE_COUNT, INTEGER | READONLY).set_default_value(0),
            coldef(COLS::DETAILS_JSON, MANDATORY)
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        identification map_id{};
        identification r_session_id{};
        enums::RepetitionAlgorithm algorithm{};
        identification note_id{};
        unixtime review_date;
        int grade{};
        string response_data;
        string notes;
        unixtime started_at;
        unixtime ended_at;
        int latency_ms{};
        int answer_change_count{0};
        string details_json{"{}"};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                map_id == other.map_id &&
                r_session_id == other.r_session_id &&
                algorithm == other.algorithm &&
                note_id == other.note_id &&
                review_date == other.review_date &&
                grade == other.grade &&
                response_data == other.response_data &&
                notes == other.notes &&
                started_at == other.started_at &&
                ended_at == other.ended_at &&
                latency_ms == other.latency_ms &&
                answer_change_count == other.answer_change_count &&
                details_json == other.details_json;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
