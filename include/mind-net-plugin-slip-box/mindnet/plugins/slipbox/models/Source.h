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
#ifndef SOURCE_H
#define SOURCE_H


#include <string>
#include <utility>

#include "../../../../../mind-net-model/mindnet/model/BaseModel.h"
#include "../enums/SourceType.h"

// ***** MACROS : START *****
#define Model Source
#define MODEL SOURCE
#define COLS columns::SourceColumns
#include "../columns/SourceColumns.h"
#include "mindnet/plugins/slipbox/enums/SourceType.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline def SOURCE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_group("Slip Box", 100)
        .set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::MAP_ID, FOREIGN_KEY | MANDATORY | READONLY),
            coldef(COLS::TITLE, MANDATORY),
            coldef(COLS::AUTHOR),
            coldef(COLS::YEAR, INTEGER),
            coldef(COLS::PUBLISHER),
            coldef(COLS::EDITION),
            coldef(COLS::PAGES),
            coldef(COLS::URL),
            coldef(COLS::TYPE, INTEGER | MANDATORY).set_enum_definition(enums::source_type_to_enum_definition())
        });

    struct Model : mindnet::model::BaseModel
    {
        int map_id{};
        string title;
        string author;
        int year{};
        string publisher;
        string edition;
        string pages;
        string url;
        enums::SourceType type{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id
                && map_id == other.map_id
                && title == other.title
                && author == other.author
                && year == other.year
                && publisher == other.publisher
                && edition == other.edition
                && pages == other.pages
                && url == other.url
                && type == other.type
                && created_at == other.created_at
                && updated_at == other.updated_at;
        }

    };
}
#undef Model
#undef MODEL
#undef COLS

#endif // SOURCE_H
