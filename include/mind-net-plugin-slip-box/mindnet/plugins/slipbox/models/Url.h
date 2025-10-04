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
#ifndef URL_H
#define URL_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model Url
#define MODEL URL
#define COLS columns::UrlColumns
#include "../columns/UrlColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def URL_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100)
        .set_columns({
            coldef(COLS::FROM_NOTE_ID, MANDATORY | READONLY).set_foreign_key("note").set_description(
                "ID of the note this url is from"),
            coldef(COLS::TO_URL, MANDATORY | READONLY).set_description("URL this url points to"),
        });

    struct Model : mindnet::model::BaseModel
    {
        int from_note_id{};
        string to_url;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                from_note_id == other.from_note_id &&
                to_url == other.to_url;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

#endif // URL_H
