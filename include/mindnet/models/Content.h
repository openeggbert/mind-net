///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef CONTENT_H
#define CONTENT_H

#include <string>

#include "misc/BaseModel.h"
#include "columns/ContentColumns.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/ContentFormat.h"

// ***** DEFINE SECTION : START *****
#define Model Content
#define MODEL CONTENT
// ***** DEFINE SECTION : END *****

namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::ContentColumns;
    using misc::def;
    using misc::coldef;

    inline def CONTENT_DEFINITION =
        def(cols::MODEL_NAME)
        .set_columns(
            {
                coldef(cols::CONTENT).set_mandatory(true),
                coldef(cols::FORMAT).set_enum_definition(enums::content_format_to_enum_definition()).
                                     set_mandatory(true),
                coldef(cols::MIME_TYPE).set_mandatory(true),
                coldef(cols::VERSION).integer(),
                coldef(cols::NODE_ID).set_foreign_key("enum")
            }
        );

struct Content : bm
{
    str content;
    mindnet::enums::ContentFormat format;
    str mime_type;
    str version;
    str node_id;

    create_model_h_methods(Model, MODEL)

    bool operator==(const Model & other) const
    {
        return id == other.id &&
            created_at == other.created_at &&
            updated_at == other.updated_at &&
            content == other.content &&
            format == other.format &&
            version == other.version &&
            node_id == other.node_id;
    }

    Content() = default;

};

}

#undef Model
#undef MODEL
#endif // CONTENT_H
