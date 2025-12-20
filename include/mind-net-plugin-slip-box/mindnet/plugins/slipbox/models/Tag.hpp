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
#define Model Tag
#define MODEL TAG
#define COLS columns::TagColumns
#include "../columns/TagColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def TAG_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box", 100)
        .set_all_rest_operations().set_title_column(COLS::TAG_TYPE_ID)
        .set_columns({
            coldef(COLS::NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::TAG_TYPE_ID, MANDATORY | READONLY | FOREIGN_KEY),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification note_id{};
        identification tag_type_id{};

        static constexpr auto fields = std::make_tuple(
            &Model::note_id,
            &Model::tag_type_id
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id && note_id == other.note_id && tag_type_id == other.tag_type_id &&
                created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS