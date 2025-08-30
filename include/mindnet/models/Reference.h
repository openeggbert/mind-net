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
#ifndef REFERENCE_H
#define REFERENCE_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Reference
#define MODEL REFERENCE
#define COLS columns::ReferenceColumns
#include "columns/ReferenceColumns.h"
// ***** MACROS : END *****

namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def REFERENCE_DEFINITION =
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
    {
        {
            NodeLinkColumns::ID, ColumnType::INTEGER, true
        }
        ,
        {
            NodeLinkColumns::CREATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            NodeLinkColumns::UPDATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            NodeLinkColumns::FROM_NODE_ID, ColumnType::INTEGER, true
        }
        ,
        {
            NodeLinkColumns::TO_NODE_ID, ColumnType::INTEGER, true
        }
        ,
        {
            NodeLinkColumns::TYPE, ColumnType::INTEGER, true
        }
        ,
        {
            NodeLinkColumns::LABEL, ColumnType::TEXT, false
        }
        ,
    }
};

struct Model : mindnet::models::bm
{
    int from_note_id;
    int to_note_id;
    string label;

    create_model_h_methods(Model, MODEL)

    bool operator==(const NodeLink& other) const
    {
        return id == other.id && from_node_id == other.from_node_id && to_node_id == other.to_node_id &&
            type == other.type && label == other.label && created_at == other.created_at && updated_at == other.
            updated_at;
    }
};

}
#undef Model
#undef MODEL
#undef COLS
#endif // REFERENCE_H