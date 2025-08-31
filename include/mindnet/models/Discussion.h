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
#ifndef DISCUSSION_H
#define DISCUSSION_H


#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** MACROS : START *****
#define Model Discussion
#define MODEL DISCUSSION
#define COLS columns::DiscussionColumns
#include "columns/DiscussionColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def DISCUSSION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_columns({
            //
            coldef(COLS::TEAM_ID, MANDATORY | FOREIGN_KEY).set_description("ID of the team this discussion belongs to"),
            coldef(COLS::TITLE, MANDATORY).set_description("Title of the discussion"),
            coldef(COLS::CREATED_BY, MANDATORY).set_foreign_key("user").set_description(
                "User ID who created the discussion"),
            coldef(COLS::IS_PINNED, BOOL).set_default_value(0).set_description(
                "Whether this discussion is pinned to the top"),
        })
    .add_custom_list_action("comment","List comments", {"discussion_id","{id}"})
    .add_custom_create_action("comment","Add comment", {"discussion_id","{id}"})
    ;

    struct Model : misc::BaseModel
    {
        int team_id{};
        string title;
        int created_by{};
        bool is_pinned{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                team_id == other.team_id &&
                title == other.title &&
                created_by == other.created_by &&
                is_pinned == other.is_pinned;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

#endif // DISCUSSION_H