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
#ifndef NODE_LINK_H
#define NODE_LINK_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
#include "columns/NodeLinkColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::NodeLinkColumns;

    static ModelDefinition NODE_LINK_DEFINITION = {
        NodeLinkColumns::MODEL_NAME,
        true,
        {
            {NodeLinkColumns::ID, ColumnType::INTEGER, true},
            {NodeLinkColumns::CREATED_AT, ColumnType::INTEGER, false},
            {NodeLinkColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {NodeLinkColumns::FROM_NODE_ID, ColumnType::INTEGER, true},
            {NodeLinkColumns::TO_NODE_ID, ColumnType::INTEGER, true},
            {NodeLinkColumns::LABEL, ColumnType::TEXT, false},
        }
    };

    struct NodeLink : BaseModel
    {
        int from_node_id;
        int to_node_id;
        str label;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return NODE_LINK_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const NodeLink& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const NodeLink& other) const
        {
            return id == other.id && from_node_id == other.from_node_id && to_node_id == other.to_node_id &&
                label == other.label && created_at == other.created_at && updated_at == other.updated_at;
        }

        NodeLink() = default;

        NodeLink(int id_, int from_node_id_, int to_node_id_, str label_,
                 unixtime created_at_,
                 unixtime updated_at_
        )
            : from_node_id(from_node_id_), to_node_id(to_node_id_), label(std::move(label_))
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // NODE_LINK_H
