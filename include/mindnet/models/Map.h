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
#ifndef MAP_H
#define MAP_H


#include <string>

#include "BaseModel.h"
#include "columns/MapColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using columns::MapColumns;
    static ModelDefinition MAP_DEFINITION = {
        columns::MapColumns::MODEL_NAME,
        true,
        {
            {MapColumns::ID, ColumnType::INTEGER, true},
            {MapColumns::NAME, ColumnType::TEXT, true},
            {MapColumns::DESCRIPTION, ColumnType::TEXT, true},
            {MapColumns::CREATED_AT, ColumnType::INTEGER, false},
        }

    };

    struct Map : BaseModel
    {
        str name;
        str description;
        unixtime created_at{};

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return MAP_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const Map& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const Map& other) const
        {
            return id == other.id && name == other.name && description == other.description && created_at == other.
                created_at;
        }

        Map() = default;

        Map(int id_, const str& name_, const str& description_, unixtime created_at_)
            : name(name_), description(description_), created_at(created_at_)
        {
            id = id_;
        }
    };
}

#endif // MAP_H
