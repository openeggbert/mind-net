///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef IDEA_H
#define IDEA_H

#include <string>
#include "../../../model/BaseModel.h"

// ***** MACROS : START *****
#define Model Idea
#define MODEL IDEA
#define COLS columns::IdeaColumns
#include "../columns/IdeaColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def IDEA_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_title_column(COLS::TITLE)
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY).set_foreign_key("user")
                                                          .set_description("User who created this idea"),
            coldef(COLS::TITLE, MANDATORY).set_description("Idea title"),
            coldef(COLS::CONTENT, TEXTAREA).set_description("Idea content"),
            coldef(COLS::CATEGORY).set_description("Idea category"),
            coldef(COLS::DUE_AT, DATETIME).set_description("Due date"),
            coldef(COLS::IS_IMPORTANT, BOOL).set_default_value(0).set_description("Whether this idea is important"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(0).set_description("Whether this idea is public"),
            coldef(COLS::IS_PINNED, BOOL).set_default_value(0).set_description("Whether this idea is pinned")
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        string title;
        string content;
        string category;
        unixtime due_at;
        bool is_important{false};
        bool is_public{false};
        bool is_pinned{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                title == other.title &&
                content == other.content &&
                category == other.category &&
                due_at == other.due_at &&
                is_important == other.is_important &&
                is_public == other.is_public &&
                is_pinned == other.is_pinned;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // IDEA_H
