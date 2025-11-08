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
#ifndef COLLECTIONITEM_H
#define COLLECTIONITEM_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model CollectionItem
#define MODEL COLLECTION_ITEM
#define COLS columns::CollectionItemColumns
#include "../columns/CollectionItemColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def COLLECTION_ITEM_DEFINITION =
        def(COLS::MODEL_NAME).set_title_column(COLS::NOTE_ID)
                             .set_all_rest_operations()
                             .set_group("Slip Box", 100)
                             .set_columns({
                                 coldef(COLS::COLLECTION_ID, FOREIGN_KEY | MANDATORY | READONLY).set_description(
                                     "Foreign key referencing the collection"),
                                 coldef(COLS::NOTE_ID, FOREIGN_KEY | MANDATORY | READONLY).set_description(
                                     "Foreign key referencing the note"),
                                 coldef(COLS::ORDER_INDEX, INTEGER).set_description(
                                     "Ordering index of the item within the collection"),
                             });

    struct Model : mindnet::model::BaseModel
    {
        int collection_id{};
        int note_id{};
        int order_index{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                collection_id == other.collection_id &&
                note_id == other.note_id &&
                order_index == other.order_index;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

#endif // COLLECTIONITEM_H
