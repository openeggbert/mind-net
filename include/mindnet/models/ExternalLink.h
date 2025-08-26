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
#ifndef EXTERNAL_LINK_H
#define EXTERNAL_LINK_H

#include <string>
#include <utility>

#include "TagType.h"
#include "misc/BaseModel.h"
#include "columns/ExternalLinkColumns.h"
#include "mindnet/Helper.h"
// ***** DEFINE SECTION : START *****
#define Model ExternalLink
#define MODEL EXTERNAL_LINK
// ***** DEFINE SECTION : END *****

namespace mindnet::models
{

    using bm = misc::BaseModel;
    using cols = columns::ExternalLinkColumns;
    using misc::def;
    using misc::coldef;

    inline def EXTERNAL_LINK_DEFINITION =
        def(cols::MODEL_NAME)
        .set_operations({enums::Crudl::READ,enums::Crudl::LIST})
        .set_columns(
        {
            coldef(cols::FROM_NODE_ID).set_foreign_key("node"),
            coldef(cols::TO_URL).set_mandatory(true)
        });

    ;

    struct ExternalLink : bm
    {
        int from_node_id;
        str to_url;
        create_model_h_methods(Model, MODEL)

        bool operator==(const ExternalLink& other) const
        {
            return id == other.id && from_node_id == other.from_node_id && to_url == other.to_url &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        ExternalLink() = default;
    };
}

#undef Model
#undef MODEL
#endif // EXTERNAL_LINK_H
