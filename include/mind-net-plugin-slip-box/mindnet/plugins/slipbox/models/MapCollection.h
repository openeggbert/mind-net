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
#ifndef MAP_COLLECTION_H
#define MAP_COLLECTION_H

#include <string>
#include <utility>

#include "mindnet/model/BaseModel.h"
// ***** MACROS : START *****
#define Model MapCollection
#define MODEL MAP_COLLECTION
#define COLS columns::MapCollectionColumns
#include "../columns/MapCollectionColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def MAP_COLLECTION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_group("Slip Box", 100)
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

#endif // MAP_COLLECTION_H
