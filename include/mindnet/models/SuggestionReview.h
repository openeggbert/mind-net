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
#ifndef SUGGESTIONREVIEW_H
#define SUGGESTIONREVIEW_H


#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** MACROS : START *****
#define Model SuggestionReview
#define MODEL SUGGESTION_REVIEW
#define COLS columns::SuggestionReviewColumns
#include "columns/SuggestionReviewColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def SUGGESTION_REVIEW_DEFINITION =
        def(cols::MODEL_NAME)
        .set_all_rest_operations()
    .set_columns({
        //
        coldef(COLS::ID,tttt MANDATORY),
        coldef(COLS::ID,tttt MANDATORY),
        coldef(COLS::ID,tttt MANDATORY),
        coldef(COLS::ID,tttt MANDATORY),
        coldef(COLS::ID,tttt MANDATORY),
        coldef(COLS::ID,tttt MANDATORY),
        coldef(COLS::ID,tttt MANDATORY),

        //
    });

    struct Model : misc::BaseModel
    {
        int suggestion_id{};
        int reviewer_id{};
        int decision_status{};
        string comment;
        string reviewed_at;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                table_name == other.table_name &&
                record_id == other.record_id &&
                operation == other.operation &&
                data_json == other.data_json &&
                reason == other.reason;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // SUGGESTIONREVIEW_H