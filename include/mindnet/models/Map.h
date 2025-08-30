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
#define COLS columns::MapColumns
#include "columns/MapColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def MAP_DEFINITION =
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
        .set_columns(
            {
                coldef(COLS::NAME).set_mandatory().set_unique(),
                coldef(COLS::DESCRIPTION),
                coldef(COLS::CATEGORY),
                coldef(COLS::OWNER_ID).set_foreign_key("user"),
                coldef(COLS::IS_PUBLIC).bool_column()

            });


    struct Model : misc::BaseModel
    {
        string name;
        string description;
        string category;
        int owner_id{};
        int team_id{};
        int owner_rights{};
        int team_rights{};
        int other_rights{};

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
#undef COLS

#endif // MAP_H