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
#ifndef MAP_H
#define MAP_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Map
#define MODEL MAP
#include "columns/MapColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::MapColumns;
    using misc::def;
    using misc::coldef;

    inline def MAP_DEFINITION =
        def(cols::MODEL_NAME)
        .set_rest_operations("crudl")
        .set_columns(
            {
                coldef(cols::NAME).set_mandatory().set_unique(),
                coldef(cols::DESCRIPTION),
                coldef(cols::CATEGORY),
                coldef(cols::OWNER_ID).set_foreign_key("user"),
                coldef(cols::IS_PUBLIC).bool_column()

            });


    struct Model : bm
    {
        Model() = default;

        str name;
        str description;
        str category;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Map& other) const
        {
            return id == other.id && name == other.name && description == other.description && category == other.
                category && created_at == other.created_at && updated_at == other.updated_at;
        }

    };
}
#undef Model
#undef MODEL

#endif // MAP_H
