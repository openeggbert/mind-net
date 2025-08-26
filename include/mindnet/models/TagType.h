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
#ifndef TAGTYPE_H
#define TAGTYPE_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
#include "columns/TagTypeColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::TagTypeColumns;
    static ModelDefinition TAG_TYPE_DEFINITION = {
        TagTypeColumns::MODEL_NAME,
        true,
        {
            {TagTypeColumns::ID, ColumnType::INTEGER, true},
            {TagTypeColumns::CREATED_AT, ColumnType::INTEGER, false},
            {TagTypeColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {TagTypeColumns::MAP_ID, ColumnType::INTEGER, true},
            {TagTypeColumns::TITLE, ColumnType::TEXT, true}
        }
    };

    struct TagType : BaseModel
    {
        int map_id;
        str title;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return TAG_TYPE_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const TagType& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const TagType& other) const
        {
            return id == other.id && map_id == other.map_id && title == other.title &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        TagType() = default;

        TagType(int id_, int map_id_, str title_,
            unixtime created_at_,
            unixtime updated_at_
        )
            : map_id(map_id_), title(std::move(title_))
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // TAGTYPE_H
