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
#ifndef NODE_PROPERTY_H
#define NODE_PROPERTY_H

#include <string>
#include <utility>

#include "misc/BaseModel.h"
#include "columns/NodePropertyColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::NodePropertyColumns;

    static ModelDefinition NODE_PROPERTY_DEFINITION = {
        NodePropertyColumns::MODEL_NAME,
        true,
        {
            {NodePropertyColumns::ID, ColumnType::INTEGER, true},
            {NodePropertyColumns::CREATED_AT, ColumnType::INTEGER, false},
            {NodePropertyColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {NodePropertyColumns::MAP_ID, ColumnType::INTEGER, true},
            {NodePropertyColumns::NODE_ID, ColumnType::INTEGER, true},
            {NodePropertyColumns::KEY, ColumnType::TEXT, true},
            {NodePropertyColumns::VALUE, ColumnType::TEXT, false},
            {NodePropertyColumns::VALUE_TYPE, ColumnType::INTEGER, false},
            {NodePropertyColumns::IS_INDEXED, ColumnType::INTEGER, false},
        }
    };

    struct NodeProperty : BaseModel
    {
        int map_id;
        int node_id;
        str key;
        str value;
        int value_type{};
        bool is_indexed{};

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return NODE_PROPERTY_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const NodeProperty& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const NodeProperty& other) const
        {
            return id == other.id && map_id == other.map_id && node_id == other.node_id && key == other.key &&
                value == other.value && value_type == other.value_type && is_indexed == other.is_indexed &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        NodeProperty() = default;

        NodeProperty(int id_, int map_id_, int node_id_, str key_, str value_, int value_type_, bool is_indexed_,
                     unixtime created_at_,
                     unixtime updated_at_
        )
            : map_id(map_id_), node_id(node_id_), key(std::move(key_)), value(std::move(value_)),
              value_type(value_type_), is_indexed(is_indexed_)
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // NODE_PROPERTY_H
