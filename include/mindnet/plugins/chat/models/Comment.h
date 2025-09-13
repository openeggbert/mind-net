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
#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include "../../../model/BaseModel.h"

// ***** MACROS : START *****
#define Model Comment
#define MODEL COMMENT
#define COLS columns::CommentColumns
#include "../columns/CommentColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::chat::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def COMMENT_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Collaboration", 300)
        .set_columns({
            //
            coldef(COLS::DISCUSSION_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description(
                "The ID of the discussion this comment belongs to"),
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description(
                "The ID of the user who created this comment"),
            coldef(COLS::CONTENT, TEXTAREA | MANDATORY).set_description("The content of the comment"),
            coldef(COLS::PARENT_COMMENT_ID, READONLY | READONLY).set_foreign_key("comment").set_description(
                "The ID of the parent comment if this is a reply"),
            coldef(COLS::IS_DELETED, BOOL).set_default_value(0).
                                           set_description("Whether this comment has been deleted"),
        })
        .add_custom_list_action("comment", "List subcomments", {"parent_comment_id", "{id}"})
        .add_custom_create_action("comment", "Add subcomment", {"parent_comment_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        int discussion_id{};
        int user_id{};
        string content;
        int parent_comment_id{};
        bool is_deleted{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                discussion_id == other.discussion_id &&
                user_id == other.user_id &&
                content == other.content &&
                parent_comment_id == other.parent_comment_id &&
                is_deleted == other.is_deleted;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

#endif // COMMENT_H
