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
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
// ***** MACROS : START *****
#define Model MapCollection
#define MODEL MAP_COLLECTION
#define COLS columns::MapCollectionColumns
#include "../columns/MapCollectionColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def MAP_COLLECTION_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box #2", 100)
        .set_all_rest_operations().set_title_column(COLS::NAME)
        .set_columns({
            coldef(COLS::NAME, MANDATORY | READONLY),
            coldef(COLS::DESCRIPTION),
            coldef(COLS::CREATED_BY, MANDATORY | READONLY).set_foreign_key("user"),
            coldef(COLS::IS_PUBLIC, BOOL).set_default_value(false),
        });

    struct Model : mindnet::model::BaseModel
    {
        string name;
        string description;
        i64 created_by{};
        bool is_public{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const MapCollection& other) const
        {
            return id == other.id &&
                name == other.name &&
                description == other.description &&
                created_by == other.created_by &&
                is_public == other.is_public &&
                created_at == other.created_at &&
                updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

