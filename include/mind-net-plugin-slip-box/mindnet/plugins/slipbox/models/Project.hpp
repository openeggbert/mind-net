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
#define Model Project
#define MODEL PROJECT
#define COLS columns::ProjectColumns
#include "../columns/ProjectColumns.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def PROJECT_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box #2", 100).set_title_column(COLS::TITLE)
        .set_columns({
            coldef(COLS::NOTE_ID, INTEGER | FOREIGN_KEY).set_description("Note ID this project is linked to"),
            coldef(COLS::TITLE, MANDATORY).set_description("Project title"),
            coldef(COLS::DESCRIPTION).set_description("Project description"),
            coldef(COLS::PROGRESS, INTEGER).set_description("Project progress (0-100)"),
            coldef(COLS::IN_PROGRESS, BOOL).set_default_value(0).set_description("Whether project is in progress"),
            coldef(COLS::IMPORTANT, BOOL).set_default_value(0).set_description("Project importance flag"),
            coldef(COLS::DUE_DATE, DATETIME).set_description("Project due date"),
            coldef(COLS::CREATED_BY, READONLY).set_foreign_key("user").set_description("User who created the project"),
            coldef(COLS::OWNER_ID).set_foreign_key("user").set_description("Project owner"),
            coldef(COLS::ASSIGNED_TO).set_foreign_key("user").set_description("User assigned to the project"),
            coldef(COLS::CATEGORY).set_description("Project category"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(0).set_description("Whether this project is public")
        });

    struct Model : mindnet::model::BaseModel
    {
        int note_id{};
        string title;
        string description;
        int progress{};
        bool in_progress{false};
        bool important{false};
        unixtime due_date;
        int created_by{};
        int owner_id{};
        int assigned_to{};
        string category;
        bool is_public{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                note_id == other.note_id &&
                title == other.title &&
                description == other.description &&
                progress == other.progress &&
                in_progress == other.in_progress &&
                important == other.important &&
                due_date == other.due_date &&
                created_by == other.created_by &&
                owner_id == other.owner_id &&
                assigned_to == other.assigned_to &&
                category == other.category &&
                is_public == other.is_public;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
