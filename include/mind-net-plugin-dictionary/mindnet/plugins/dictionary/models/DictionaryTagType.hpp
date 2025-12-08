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

#include "mindnet/model/BaseModel.hpp"
#include "mindnet/plugins/dictionary/SlipBoxPlugin.hpp"
// ***** MACROS : START *****
#define Model TagType
#define MODEL TAG_TYPE
#define COLS columns::TagTypeColumns
#include "../columns/TagTypeColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def TAG_TYPE_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100).set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::MAP_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::TITLE, MANDATORY | READONLY),
            //
        })
        .add_custom_list_action("tag", "List tags", {"tag_type_id", "{id}"})
        .add_custom_create_action("tag", "Add tag", {"tag_type_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        identification map_id;
        string title;

        create_model_h_methods(Model, MODEL)

        bool operator==(const TagType& other) const
        {
            return id == other.id && map_id == other.map_id && title == other.title &&
                created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS