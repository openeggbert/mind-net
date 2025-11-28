/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model Idea
#define MODEL IDEA
#define COLS columns::IdeaColumns
#include "../columns/IdeaColumns.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def IDEA_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_title_column(COLS::TITLE)
        .allow_reader_write()
        .set_group("Slip Box", 100)
        .set_columns({
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY).set_foreign_key("user")
                                                                     .set_description("User who created this idea"),
            coldef(COLS::TITLE, MANDATORY).set_description("Idea title"),
            coldef(COLS::CONTENT, TEXTAREA).set_description("Idea content"),
            coldef(COLS::CATEGORY).set_description("Idea category"),
            coldef(COLS::DUE_AT, DATETIME).set_description("Due date"),
            coldef(COLS::IMPORTANT, BOOL).set_default_value(0).set_description("Whether this idea is important"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(0).set_description("Whether this idea is public"),
            coldef(COLS::PINNED, BOOL).set_default_value(0).set_description("Whether this idea is pinned")
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        string title;
        string content;
        string category;
        unixtime due_at;
        bool important{false};
        bool is_public{false};
        bool pinned{false};

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
                important == other.important &&
                is_public == other.is_public &&
                pinned == other.pinned;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
