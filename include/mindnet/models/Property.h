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
#ifndef PROPERTY_H
#define PROPERTY_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Property
#define MODEL PROPERTY
#define COLS columns::PropertyColumns
#include "columns/PropertyColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def PROPERTY_DEFINITION =
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
            PropertyColumns::ID, ColumnType::INTEGER, true
        }
        ,
        {
            PropertyColumns::CREATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            PropertyColumns::UPDATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            PropertyColumns::MAP_ID, ColumnType::INTEGER, true
        }
        ,
        {
            PropertyColumns::NODE_ID, ColumnType::INTEGER, true
        }
        ,
        {
            PropertyColumns::KEY, ColumnType::TEXT, true
        }
        ,
        {
            PropertyColumns::VALUE, ColumnType::TEXT, false
        }
        ,
        {
            PropertyColumns::VALUE_TYPE, ColumnType::INTEGER, false
        }
        ,
        {
            PropertyColumns::IS_INDEXED, ColumnType::INTEGER, false
        }
        ,
    }
};

struct Model : misc::BaseModel
{
    int map_id;
    int note_id;
    string key;
    string value;

    create_model_h_methods(Model, MODEL)

    bool operator==(const Property& other) const
    {
        return id == other.id && map_id == other.map_id && node_id == other.node_id && key == other.key &&
            value == other.value && value_type == other.value_type && is_indexed == other.is_indexed &&
            created_at == other.created_at && updated_at == other.updated_at;
    }
};

}
#undef Model
#undef MODEL
#undef COLS
#endif // PROPERTY_H