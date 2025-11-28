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
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : START *****
#define Model PinnedNote
#define MODEL PINNED_NOTE
#define COLS columns::PinnedNoteColumns
#include "../columns/PinnedNoteColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def PINNED_NOTE_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100)
        .set_columns({
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY)
            .set_description("User who pinned the note"),
            coldef(COLS::NOTE_ID, MANDATORY | FOREIGN_KEY | READONLY)
            .set_description("ID of the pinned note"),
            coldef(COLS::POSITION, INTEGER)
            .set_description("Optional position for ordering pinned notes"),
            coldef(COLS::IS_PUBLIC, BOOL)
            .set_default_value(0)
            .set_description("Whether this pinned note is public"),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        identification note_id{};
        int position{};
        bool is_public{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                position == other.position &&
                is_public == other.is_public;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
