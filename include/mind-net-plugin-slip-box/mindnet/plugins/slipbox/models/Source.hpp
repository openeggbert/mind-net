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
#include "../enums/SourceType.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : START *****
#define Model Source
#define MODEL SOURCE
#define COLS columns::SourceColumns
#include "../columns/SourceColumns.hpp"
#include "mindnet/plugins/slipbox/enums/SourceType.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def SOURCE_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
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

