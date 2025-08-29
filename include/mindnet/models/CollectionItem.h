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
#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** DEFINE TWO MACROS : START *****
#define Model History
#define MODEL HISTORY
// ***** DEFINE TWO MACROS : END *****
#include "columns/HistoryColumns.h"

namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::HistoryColumns;
    using misc::def;
    using misc::coldef;

    inline def HISTORY_DEFINITION =
        def(cols::MODEL_NAME)
        .set_rest_operations({mindnet::enums::Crudl::READ, mindnet::enums::Crudl::LIST})
        .set_columns({
            //
            coldef(cols::USER_ID).set_mandatory().set_foreign_key("user"),
            coldef(cols::IP_ADDRESS),
            coldef(cols::TABLE_NAME).set_mandatory(),
            coldef(cols::RECORD_ID).set_mandatory(),
            coldef(cols::OPERATION).set_mandatory().set_enum_definition(
                enums::crudl_to_enum_definition()),
            coldef(cols::DATA_JSON).set_mandatory(),
            coldef(cols::REASON),
            //
        });

    struct History : bm
    {
        History() = default;

        str table_name;
        int record_id{};
        enums::Crudl operation{};
        str data_json;
        str reason;

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
#endif // HISTORY_H
