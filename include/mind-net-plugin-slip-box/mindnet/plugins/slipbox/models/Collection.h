///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef COLLECTION_H
#define COLLECTION_H

#include <string>
#include "../../../../../mind-net-model/mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model Collection
#define MODEL COLLECTION
#define COLS columns::CollectionColumns
#include "../columns/CollectionColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def COLLECTION_DEFINITION =
        def(COLS::MODEL_NAME)
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

    struct Model : mindnet::model::BaseModel
    {
        string name;
        string description;
        int order_index{};
        int created_by{};
        bool is_public{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                name == other.name &&
                description == other.description &&
                order_index == other.order_index &&
                created_by == other.created_by &&
                is_public == other.is_public;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // COLLECTION_H
