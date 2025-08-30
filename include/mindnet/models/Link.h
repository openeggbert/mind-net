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
#ifndef LINK_H
#define LINK_H


#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** MACROS : START *****
#define Model Link
#define MODEL LINK
#define COLS columns::LinkColumns
#include "columns/LinkColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def LINK_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
    .set_columns({
    //
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),

    //
});
        .set_columns({
            //
            coldef(COLS::USER_ID).set_mandatory().set_foreign_key("user"),
            coldef(COLS::IP_ADDRESS),
            coldef(COLS::TABLE_NAME).set_mandatory(),
            coldef(COLS::RECORD_ID).set_mandatory(),
            coldef(COLS::OPERATION).set_mandatory().set_enum_definition(
                enums::crudl_to_enum_definition()),
            coldef(COLS::DATA_JSON).set_mandatory(),
            coldef(COLS::REASON),
            //
        });

    struct Model : misc::BaseModel
    {
        int from_note_id{};
        string to_url;

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

#endif // LINK_H