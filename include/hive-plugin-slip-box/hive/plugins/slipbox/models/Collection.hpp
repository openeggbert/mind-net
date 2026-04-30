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
#include "hive/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model Collection
#define MODEL COLLECTION
#define COLS columns::CollectionColumns
#include "../columns/CollectionColumns.hpp"
#include "hive/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def COLLECTION_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100).set_title_column(COLS::NAME)
        .set_columns({
            coldef(COLS::NAME, MANDATORY).set_description("Collection name"),
            coldef(COLS::DESCRIPTION).set_description("Collection description"),
            coldef(COLS::ORDER_INDEX, INTEGER).set_description("Order index for sorting"),
            coldef(COLS::CREATED_BY, READONLY).set_foreign_key("user").set_description(
                "User ID who created this collection"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(0).set_description("Whether this collection is public"),
        })
        .add_custom_list_action("collection_item", "List items", {"collection_id", "{id}"})
        .add_custom_create_action("collection_item", "Add item", {"collection_id", "{id}"});

    struct Model : hive::model::BaseModel
    {
        string name;
        string description;
        int order_index{};
        identification created_by{};
        bool is_public{false};

        static constexpr auto fields = std::make_tuple(
            &Model::name,
            &Model::description,
            &Model::order_index,
            &Model::created_by,
            &Model::is_public
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS