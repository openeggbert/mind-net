///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Lightweight wiki inspired by MediaWiki.
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
#ifndef CONTENT_H
#define CONTENT_H

#include <string>

#include "BaseModel.h"
#include "columns/ContentColumns.h"
#include "crow/json.h"
#include "miniwiki/Helper.h"

namespace miniwiki::models
{

    using enums::ColumnType;
    using columns::ContentColumns;
    static ModelDefinition CONTENT_DEFINITION = {
        ContentColumns::MODEL_NAME,
        true,
        {
            {ContentColumns::ID, ColumnType::INTEGER},
            {ContentColumns::CONTENT, ColumnType::TEXT},
            {ContentColumns::FORMAT, ColumnType::TEXT},
            {ContentColumns::CREATED_AT, ColumnType::INTEGER},
        }

    };
    struct Content : BaseModel
    {
        str content;
        str format;
        unixtime created_at{};

        [[nodiscard]] inline ModelDefinition get_definition() const override
        {
            return CONTENT_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;

        friend std::ostream& operator<<(std::ostream& os, const Content& idea)
        {
            os << idea.to_json();
            return os;
        }

        bool operator==(const Content& other) const
        {
            return id == other.id && content == other.content && format == other.format && created_at == other.
                created_at;
        }

        Content() = default;

        Content(int i, const str& s, const str& string, int i1)
        {
            id = i;
            content = s;
            format = string;
            created_at = i1;
        }
    };
}

#endif // CONTENT_H
