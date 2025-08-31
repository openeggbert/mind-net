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
            coldef(COLS::NAME, MANDATORY | UNIQUE),
            coldef(COLS::DESCRIPTION),
            coldef(COLS::CATEGORY),
            coldef(COLS::OWNER_ID, MANDATORY).set_foreign_key("user"),
            coldef(COLS::TEAM_ID, FOREIGN_KEY),
            coldef(COLS::OWNER_RIGHTS, INTEGER | MANDATORY).set_default_value("7"),
            coldef(COLS::TEAM_RIGHTS, INTEGER | MANDATORY).set_default_value("7"),
            coldef(COLS::OTHER_RIGHTS, INTEGER | MANDATORY).set_default_value("7")
        })
    .add_custom_list_action("note","List notes", {"map_id","{id}"})
    .add_custom_list_action("note","Create note", {"map_id","{id}"})
    .add_custom_list_action("tag_type","List tags", {"map_id","{id}"})
    .add_custom_list_action("tag_type","Create tag", {"map_id","{id}"});


    struct Model : misc::BaseModel
    {
        string name;
        string description;
        string category;
        int owner_id{};
        int team_id{};
        int owner_rights{7};
        int team_rights{7};
        int other_rights{7};

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
