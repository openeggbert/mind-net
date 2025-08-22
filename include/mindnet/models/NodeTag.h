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
#ifndef NODE_TAG_H
#define NODE_TAG_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
#include "columns/NodeTagColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::NodeTagColumns;

    static ModelDefinition NODE_TAG_DEFINITION = {
        NodeTagColumns::MODEL_NAME,
        true,
        {
            {NodeTagColumns::ID, ColumnType::INTEGER, true},
            {NodeTagColumns::CREATED_AT, ColumnType::INTEGER, false},
            {NodeTagColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {NodeTagColumns::NODE_ID, ColumnType::INTEGER, true},
            {NodeTagColumns::TAG_ID, ColumnType::INTEGER, true},
        }
    };

    struct NodeTag : BaseModel
    {
        int node_id;
        int tag_id;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return NODE_TAG_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const NodeTag& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const NodeTag& other) const
        {
            return id == other.id && node_id == other.node_id && tag_id == other.tag_id &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        NodeTag() = default;

        NodeTag(int id_, int node_id_, int tag_id_,
                unixtime created_at_,
                unixtime updated_at_
        )
        {
            id = id_;
            node_id = node_id_;
            tag_id = tag_id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // NODE_TAG_H
