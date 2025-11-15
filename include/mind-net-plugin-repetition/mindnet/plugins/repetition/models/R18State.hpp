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
#ifndef R18STATE_H
#define R18STATE_H

#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model R18State
#define MODEL R18_STATE
#define COLS columns::R18StateColumns
#include "../columns/R18StateColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R18_STATE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_group("Repetition", 200).set_title_column(COLS::STABILITY_TIMES_100).allow_reader_write()
        .set_rest_operations("rl")
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | READONLY),

            coldef(COLS::STABILITY_TIMES_100, INTEGER).set_default_value(100),
            coldef(COLS::LAST_INTERVAL_TIMES_100, INTEGER).set_default_value(0),
            coldef(COLS::REPETITIONS, INTEGER).set_default_value(0),
            coldef(COLS::LAPSES, INTEGER).set_default_value(0),
            coldef(COLS::NEXT_REVIEW, DATETIME),
            coldef(COLS::LAST_REVIEW, DATETIME),
            coldef(COLS::LAST_QUALITY, INTEGER).set_default_value(0),
            coldef(COLS::LAST_SEEN_SEMANTIC_VERSION, INTEGER).set_default_value(1),
            coldef(COLS::CONTENT_MODIFIED_SINCE_LAST_REVIEW, BOOL).set_default_value(false),
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int note_id{};
        int stability_times_100{100};
        int last_interval_times_100{0};
        int repetitions{0};
        int lapses{0};
        unixtime next_review;
        unixtime last_review;
        int last_quality{0};
        int last_seen_semantic_version{};
        bool content_modified_since_last_review{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                stability_times_100 == other.stability_times_100 &&
                last_interval_times_100 == other.last_interval_times_100 &&
                repetitions == other.repetitions &&
                lapses == other.lapses &&
                next_review == other.next_review &&
                last_review == other.last_review &&
                last_quality == other.last_quality &&
                last_seen_semantic_version == other.last_seen_semantic_version &&
                content_modified_since_last_review == other.content_modified_since_last_review;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // R18STATE_H
