///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef SM2STATE_H
#define SM2STATE_H

#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** MACROS : START *****
#define Model SM2State
#define MODEL SM2STATE
#define COLS columns::SM2StateColumns
#include "columns/SM2StateColumns.h"
// ***** MACROS : END *****

namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def SM2STATE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
    .set_columns({
        //
        coldef(COLS::USER_ID,FOREIGN_KEY | MANDATORY),
        coldef(COLS::NOTE_ID,FOREIGN_KEY | MANDATORY),
        coldef(COLS::REPETITIONS,INTEGER).set_default_value(0),
        coldef(COLS::INTERVAL,INTEGER).set_default_value(1),
        coldef(COLS::EF_TIMES_100,INTEGER).set_default_value(250),
        coldef(COLS::NEXT_REVIEW,DATETIME),
        coldef(COLS::LAST_REVIEW,DATETIME),
        coldef(COLS::LAST_QUALITY,INTEGER).set_default_value(0),
    });

    struct Model : misc::BaseModel
    {
        int user_id{};
        int note_id{};
        int repetitions{};
        int interval{1};
        int ef_times_100{250};
        string next_review;
        string last_review;
        int last_quality{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                repetitions == other.repetitions &&
                interval == other.interval &&
                ef_times_100 == other.ef_times_100 &&
                next_review == other.next_review &&
                last_review == other.last_review &&
                last_quality == other.last_quality;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // SM2STATE_H