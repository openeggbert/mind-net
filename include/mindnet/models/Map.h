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
#include <utility>

#include "misc/BaseModel.h"
#include "columns/MapColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::MapColumns;

    static ModelDefinition MAP_DEFINITION = {
        MapColumns::MODEL_NAME,
        true,
        {
            {MapColumns::ID, ColumnType::INTEGER, true},
            {MapColumns::CREATED_AT, ColumnType::INTEGER, false},
            {MapColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {MapColumns::NAME, ColumnType::TEXT, true},
            {MapColumns::DESCRIPTION, ColumnType::TEXT, false},
            {MapColumns::CATEGORY, ColumnType::TEXT, false},
        }
    };

    struct Map : BaseModel
    {
        str name;
        str description;
        str category;

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
            return id == other.id && name == other.name && description == other.description && category == other.
                category && created_at == other.created_at && updated_at == other.updated_at;
        }

        Map() = default;

        Map(int id_, str name_, str description_, str category_,
            unixtime created_at_,
            unixtime updated_at_
        )
            : name(std::move(name_)), description(std::move(description_)), category(std::move(category_))
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // MAP_H
