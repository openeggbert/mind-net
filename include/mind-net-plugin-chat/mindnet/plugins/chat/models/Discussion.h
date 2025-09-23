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
#ifndef DISCUSSION_H
#define DISCUSSION_H

#include <string>
#include "../../../../../mind-net-model/mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model Discussion
#define MODEL DISCUSSION
#define COLS columns::DiscussionColumns
#include "../columns/DiscussionColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::chat::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def DISCUSSION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Chat", 300)
        .set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::TEAM_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description(
                "ID of the team this discussion belongs to"),
            coldef(COLS::TITLE, MANDATORY).set_description("Title of the discussion"),
            coldef(COLS::CREATED_BY, MANDATORY | READONLY).set_foreign_key("user").set_description(
                "User ID who created the discussion"),
            coldef(COLS::IS_PINNED, BOOL).set_default_value(0).set_description(
                "Whether this discussion is pinned to the top"),
            coldef(COLS::IS_ARCHIVED, BOOL).set_default_value(0).set_description(
                "Whether this discussion is archived or not"),
        })
        .add_custom_list_action("comment", "List comments", {"discussion_id", "{id}"})
        .add_custom_create_action("comment", "Add comment", {"discussion_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        int team_id{};
        string title;
        int created_by{};
        bool is_pinned{};
        bool is_archived{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                team_id == other.team_id &&
                title == other.title &&
                created_by == other.created_by &&
                is_pinned == other.is_pinned &&
                is_archived == other.is_archived;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

#endif // DISCUSSION_H
