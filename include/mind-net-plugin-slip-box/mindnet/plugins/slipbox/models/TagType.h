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
#ifndef TAGTYPE_H
#define TAGTYPE_H

#include <string>
#include <utility>

#include "mindnet/model/BaseModel.h"
// ***** MACROS : START *****
#define Model TagType
#define MODEL TAG_TYPE
#define COLS columns::TagTypeColumns
#include "../columns/TagTypeColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def TAG_TYPE_DEFINITION =
        def(COLS::MODEL_NAME)
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
        int map_id;
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
#endif // TAGTYPE_H
