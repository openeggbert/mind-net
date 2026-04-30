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
#include "hive/plugins/dictionary/DictionaryPlugin.hpp"

// ***** MACROS : START *****
#define Model DictionaryReview
#define MODEL DICTIONARY_REVIEW
#define COLS columns::DictionaryReviewColumns
#include "../columns/DictionaryReviewColumns.hpp"
#include "hive/plugins/dictionary/enums/RepetitionAlgorithm.hpp"

// ***** MACROS : END *****

namespace hive::plugins::dictionary::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def DICTIONARY_REVIEW_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_rest_operations("crdl")
        .set_group("Dictionary", 100).set_title_column(COLS::DICTIONARY_TERM_ID)
        .allow_reader_write().set_readonly()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::DICTIONARY_MAP_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::ALGORITHM, MANDATORY)
            .set_enum_definition(enums::repetition_algorithm_to_enum_definition())
            .set_default_value(18),
            coldef(COLS::DICTIONARY_TERM_ID, FOREIGN_KEY),
            coldef(COLS::REVIEW_DATE, DATETIME),
            coldef(COLS::GRADE, INTEGER),
            coldef(COLS::STARTED_AT, DATETIME),
            coldef(COLS::ENDED_AT, DATETIME),
            coldef(COLS::LATENCY_MS, INTEGER),
            coldef(COLS::ANSWER_CHANGE_COUNT, INTEGER).set_default_value(0),
            coldef(COLS::DETAILS_JSON, MANDATORY | MUTABLE)
        });

    struct Model : hive::model::BaseModel
    {
        identification user_id{};
        identification dictionary_map_id{};
        enums::RepetitionAlgorithm algorithm{};
        identification dictionary_term_id{};
        unixtime review_date{};
        int grade{};
        unixtime started_at{};
        unixtime ended_at{};
        int latency_ms{};
        int answer_change_count{0};
        string details_json{"{}"};

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::dictionary_map_id,
            &Model::algorithm,
            &Model::dictionary_term_id,
            &Model::review_date,
            &Model::grade,
            &Model::started_at,
            &Model::ended_at,
            &Model::latency_ms,
            &Model::answer_change_count,
            &Model::details_json
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS