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
#ifndef PROPERTY_H
#define PROPERTY_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Property
#define MODEL PROPERTY
#include "columns/PropertyColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::PropertyColumns;
    using misc::def;
    using misc::coldef;

    inline def PROPERTY_DEFINITION =
        def(cols::MODEL_NAME)
        .set_rest_operations("crudl")
        {
            {PropertyColumns::ID, ColumnType::INTEGER, true},
            {PropertyColumns::CREATED_AT, ColumnType::INTEGER, false},
            {PropertyColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {PropertyColumns::MAP_ID, ColumnType::INTEGER, true},
            {PropertyColumns::NODE_ID, ColumnType::INTEGER, true},
            {PropertyColumns::KEY, ColumnType::TEXT, true},
            {PropertyColumns::VALUE, ColumnType::TEXT, false},
            {PropertyColumns::VALUE_TYPE, ColumnType::INTEGER, false},
            {PropertyColumns::IS_INDEXED, ColumnType::INTEGER, false},
        }
    };

struct Model : bm
{
    Model() = default;

        int map_id;
        int node_id;
        str key;
        str value;
        enums::ValueType value_type{};
        bool is_indexed{};

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return PROPERTY_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const Property& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const Property& other) const
        {
            return id == other.id && map_id == other.map_id && node_id == other.node_id && key == other.key &&
                value == other.value && value_type == other.value_type && is_indexed == other.is_indexed &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        Property() = default;

        Property(int id_, int map_id_, int node_id_, str key_, str value_, int value_type_, bool is_indexed_,
                     unixtime created_at_,
                     unixtime updated_at_
        )
            : map_id(map_id_), node_id(node_id_), key(std::move(key_)), value(std::move(value_)),
              value_type(static_cast<enums::ValueType>(value_type_)), is_indexed(is_indexed_)
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // PROPERTY_H
