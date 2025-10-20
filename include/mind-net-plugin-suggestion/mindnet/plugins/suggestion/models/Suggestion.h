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
#ifndef SUGGESTION_H
#define SUGGESTION_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model Suggestion
#define MODEL SUGGESTION
#define COLS columns::SuggestionColumns
#include "../columns/SuggestionColumns.h"
#include "../enums/SuggestionStatus.h"
#include "mindnet/plugins/core/enums/Crudl.h"
// ***** MACROS : END *****

namespace mindnet::plugins::suggestion::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def SUGGESTION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Suggestions", 400).set_title_column(COLS::TABLE_NAME).allow_reader_write()
        .set_columns({
            //
            coldef(COLS::PARENT_SUGGESTION_ID).set_foreign_key("suggestion"),
            coldef(COLS::FROM_USER_ID, MANDATORY | READONLY).set_foreign_key("user"),
            coldef(COLS::TABLE_NAME, MANDATORY | READONLY),
            coldef(COLS::OPERATION, MANDATORY | READONLY).set_enum_definition(core::enums::crudl_to_enum_definition()),
            coldef(COLS::STATUS).set_default_value(0).
                                 set_enum_definition(enums::suggestion_status_to_enum_definition()),
            coldef(COLS::DATA_JSON),
            coldef(COLS::REVIEW_COUNT, INTEGER).set_default_value(0),
            //
        });

    struct Model : mindnet::model::BaseModel
    {
        int parent_suggestion_id{};
        int from_user_id;
        string table_name;
        mindnet::essential::Crudl operation{};
        enums::SuggestionStatus status{};
        string data_json;
        int review_count{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                parent_suggestion_id == other.parent_suggestion_id &&
                from_user_id == other.from_user_id &&
                table_name == other.table_name &&
                operation == other.operation &&
                status == other.status &&
                data_json == other.data_json &&
                review_count == other.review_count;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // SUGGESTION_H
