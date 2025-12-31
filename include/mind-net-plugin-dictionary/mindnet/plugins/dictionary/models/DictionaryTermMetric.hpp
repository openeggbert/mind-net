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

#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model DictionaryTermMetric
#define MODEL DICTIONARY_TERM_METRIC
#define COLS columns::DictionaryTermMetricColumns
#include "../columns/DictionaryTermMetricColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_TERM_METRIC_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 210)
        .set_rest_operations("rl")
        .set_no_table(true)
        .set_cache_enabled(false)
        .set_title_column(COLS::DICTIONARY_TERM_ID)
        .set_readonly()
        .set_columns({

            coldef(COLS::DICTIONARY_TERM_ID, MANDATORY | FOREIGN_KEY)
                .set_description("Dictionary term this metric belongs to."),

            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY)
                .set_description("Dictionary map this metric belongs to."),

            coldef(COLS::TAG_COUNT, INTEGER),
            coldef(COLS::FLAG_COUNT, INTEGER),
            coldef(COLS::ALIAS_COUNT, INTEGER),
            coldef(COLS::NOTE_COUNT, INTEGER),
            coldef(COLS::LINK_COUNT, INTEGER),
            coldef(COLS::BACKLINK_COUNT, INTEGER),
            coldef(COLS::URL_COUNT, INTEGER),
            coldef(COLS::SOURCE_COUNT, INTEGER),
            coldef(COLS::INDEX_COUNT, INTEGER),
            coldef(COLS::REVIEW_COUNT, INTEGER),
            coldef(COLS::STATE_18_COUNT, INTEGER),

            coldef(COLS::UPDATE_COUNT, INTEGER),
            coldef(COLS::VIEW_COUNT, INTEGER),
            coldef(COLS::LAST_VIEWED_AT, DATETIME),
            coldef(COLS::LAST_UPDATED_AT, DATETIME),
        });

    struct DictionaryTermMetric : mindnet::model::BaseModel
    {
        identification dictionary_term_id{};
        identification dictionary_map_id{};

        int tag_count{};
        int flag_count{};
        int alias_count{};
        int note_count{};
        int link_count{};
        int backlink_count{};
        int url_count{};
        int source_count{};
        int index_count{};
        int review_count{};
        int state_18_count{};

        int update_count{};
        int view_count{};
        unixtime last_viewed_at{};
        unixtime last_updated_at{};

        static constexpr auto fields = std::make_tuple(
            &Model::dictionary_term_id,
            &Model::dictionary_map_id,

            &Model::tag_count,
            &Model::flag_count,
            &Model::alias_count,
            &Model::note_count,
            &Model::link_count,
            &Model::backlink_count,
            &Model::url_count,
            &Model::source_count,
            &Model::index_count,
            &Model::review_count,
            &Model::state_18_count,

            &Model::update_count,
            &Model::view_count,
            &Model::last_viewed_at,
            &Model::last_updated_at
        );

        create_model_h_methods(Model, MODEL)
    };
}

#undef Model
#undef MODEL
#undef COLS
