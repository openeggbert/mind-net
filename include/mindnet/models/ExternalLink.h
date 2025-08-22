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

#include "misc/BaseModel.h"
#include "columns/ExternalLinkColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::ExternalLinkColumns;

    static ModelDefinition EXTERNAL_LINK_DEFINITION = {
        ExternalLinkColumns::MODEL_NAME,
        true,
        {
            {ExternalLinkColumns::ID, ColumnType::INTEGER, true},
            {ExternalLinkColumns::CREATED_AT, ColumnType::INTEGER, false},
            {ExternalLinkColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {ExternalLinkColumns::FROM_NODE_ID, ColumnType::INTEGER, true},
            {ExternalLinkColumns::TO_URL, ColumnType::TEXT, true},
        },
        {enums::Crudl::READ,enums::Crudl::LIST}
    };

    struct ExternalLink : BaseModel
    {
        int from_node_id;
        str to_url;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return EXTERNAL_LINK_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const ExternalLink& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const ExternalLink& other) const
        {
            return id == other.id && from_node_id == other.from_node_id && to_url == other.to_url &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        ExternalLink() = default;

        ExternalLink(int id_, int from_node_id_, str to_url_,
                     unixtime created_at_,
                     unixtime updated_at_
        )
            : from_node_id(from_node_id_), to_url(std::move(to_url_))
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // EXTERNAL_LINK_H
