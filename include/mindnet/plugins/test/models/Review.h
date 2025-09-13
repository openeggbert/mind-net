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
#ifndef REVIEW_H
#define REVIEW_H

#include <string>
#include "../../../model/BaseModel.h"

// ***** MACROS : START *****
#define Model Review
#define MODEL REVIEW
#define COLS columns::ReviewColumns
#include "../columns/ReviewColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::test::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def REVIEW_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Tests", 200)
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::NOTE_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::QUESTION_ID, FOREIGN_KEY | READONLY),
            coldef(COLS::REVIEW_DATE, DATETIME),
            coldef(COLS::GRADE, INTEGER | READONLY),
            coldef(COLS::RESPONSE_DATA),
            coldef(COLS::NOTES),
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int note_id{};
        int question_id{};
        unixtime review_date;
        int grade{};
        string response_data;
        string notes;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                question_id == other.question_id &&
                review_date == other.review_date &&
                grade == other.grade &&
                response_data == other.response_data &&
                notes == other.notes;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // REVIEW_H
