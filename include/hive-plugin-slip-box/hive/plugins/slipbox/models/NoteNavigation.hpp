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
#define Model NoteNavigation
#define MODEL NOTE_NAVIGATION
#define COLS columns::NoteNavigationColumns
#include "../columns/NoteNavigationColumns.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def NOTE_NAVIGATION_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box #2", 1000)
        .set_rest_operations("r").set_title_column(COLS::ID)
        .set_no_table(true)
        .set_cache_enabled(false)
        .set_columns({
            //
            coldef(COLS::NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::PREV_NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY).set_foreign_key("note"),
            coldef(COLS::NEXT_NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY).set_foreign_key("note"),
        });

    struct Model : hive::model::BaseModel
    {
        identification note_id{};
        identification prev_note_id{};
        identification next_note_id{};

        static constexpr auto fields = std::make_tuple(
            &Model::note_id,
            &Model::prev_note_id,
            &Model::next_note_id
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS