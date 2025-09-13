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

#include "../../../model/BaseModel.h"
// ***** MACROS : START *****
#define Model Property
#define MODEL PROPERTY
#define COLS columns::PropertyColumns
#include "../columns/PropertyColumns.h"
// ***** MACROS : END *****


namespace mindnet::plugins::zettelkasten::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def PROPERTY_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("rl")
        .set_columns({
            //
            coldef(COLS::MAP_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::NOTE_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::KEY, MANDATORY | READONLY),
            coldef(COLS::VALUE),
            //
        });

    struct Model : mindnet::model::BaseModel
    {
        int map_id{};
        int note_id{};
        string key;
        string value;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Property& other) const
        {
            return id == other.id && map_id == other.map_id && note_id == other.note_id && key == other.key &&
                value == other.value && created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // PROPERTY_H
