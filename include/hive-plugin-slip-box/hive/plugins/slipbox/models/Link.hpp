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
#include <utility>

#include "hive/model/BaseModel.hpp"
#include "hive/plugins/slipbox/SlipBoxPlugin.hpp"
// ***** MACROS : START *****
#define Model Link
#define MODEL LINK
#define COLS columns::LinkColumns
#include "../columns/LinkColumns.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def LINK_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Slip Box", 100)
        .set_columns({
            coldef(COLS::FROM_NOTE_ID, MANDATORY | READONLY).set_foreign_key("note"),
            coldef(COLS::TO_NOTE_ID, MANDATORY | READONLY).set_foreign_key("note"),
            coldef(COLS::TO_NOTE_TITLE, MANDATORY | READONLY),
            coldef(COLS::LABEL),
        });
    ;

    struct Model : hive::model::BaseModel
    {
        identification from_note_id;
        identification to_note_id;
        string to_note_title;
        string label;

        static constexpr auto fields = std::make_tuple(
            &Model::from_note_id,
            &Model::to_note_id,
            &Model::to_note_title,
            &Model::label
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS