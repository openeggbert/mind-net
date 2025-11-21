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
#pragma once


#include <string>
#include <utility>

#include "mindnet/model/BaseModel.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
// ***** MACROS : START *****
#define Model MapCollectionItem
#define MODEL MAP_COLLECTION_ITEM
#define COLS columns::MapCollectionItemColumns
#include "../columns/MapCollectionItemColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def MAP_COLLECTION_ITEM_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_group("Slip Box", 100)
        .set_all_rest_operations()
        .set_columns({
            coldef(COLS::MAP_COLLECTION_ID, MANDATORY | FOREIGN_KEY | READONLY)
            .set_foreign_key("map_collection"),
            coldef(COLS::MAP_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::POSITION, INTEGER),
        });

    struct Model : mindnet::model::BaseModel
    {
        i64 map_collection_id{};
        i64 map_id{};
        i64 position{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const MapCollectionItem& other) const
        {
            return id == other.id &&
                map_collection_id == other.map_collection_id &&
                map_id == other.map_id &&
                position == other.position &&
                created_at == other.created_at &&
                updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

