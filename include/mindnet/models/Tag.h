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
#ifndef TAG_H
#define TAG_H


#include <string>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Tag
#define MODEL TAG
#include "columns/TagColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::TagColumns;
    using misc::def;
    using misc::coldef;

    inline def TAG_DEFINITION =
        def(cols::MODEL_NAME)
        .set_rest_operations("crudl")
    {
        {
            TagColumns::ID, ColumnType::INTEGER, true
        }
        ,
        {
            TagColumns::CREATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            TagColumns::UPDATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            TagColumns::NODE_ID, ColumnType::INTEGER, true
        }
        ,
        {
            TagColumns::TAG_TYPE_ID, ColumnType::INTEGER, true
        }
        ,
    }
};

struct Model : bm
{
    Model() = default;

    int node_id;
    int tag_type_id;

    [[nodiscard]] ModelDefinition get_definition() const override
    {
        return NODE_TAG_DEFINITION;
    }

    [[nodiscard]] entity_fields get_values() const override;
    void from_values(const entity_fields& values) override;

    friend std::ostream& operator<<(std::ostream& os, const Tag& map)
    {
        os << map.to_json();
        return os;
    }

    bool operator==(const Tag& other) const
    {
        return id == other.id && node_id == other.node_id && tag_type_id == other.tag_type_id &&
            created_at == other.created_at && updated_at == other.updated_at;
    }

    Tag() = default;

    Tag(int id_, int node_id_, int tag_id_,
        unixtime created_at_,
        unixtime updated_at_
    )
    {
        id = id_;
        node_id = node_id_;
        tag_type_id = tag_id_;
        created_at = created_at_;
        updated_at = updated_at_;
    }
};

}

#endif // TAG_H
