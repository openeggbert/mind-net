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
#ifndef CONTENT_H
#define CONTENT_H

#include <string>

#include "misc/BaseModel.h"
#include "columns/ContentColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/ContentFormat.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::ContentColumns;

    static ModelDefinition CONTENT_DEFINITION = {
        ContentColumns::MODEL_NAME,
        true,
        {
            {ContentColumns::ID, ColumnType::INTEGER, true},
            {ContentColumns::CREATED_AT, ColumnType::INTEGER, false},
            {ContentColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {ContentColumns::CONTENT, ColumnType::TEXT, true},
            {ContentColumns::FORMAT, ColumnType::TEXT, true},
            {ContentColumns::VERSION, ColumnType::INTEGER, true},
            {ContentColumns::NODE_ID, ColumnType::INTEGER, true},
        }

    };

    struct Content : BaseModel
    {
        str content;
        enums::ContentFormat format;
        str version;
        str node_id;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return CONTENT_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const Content& idea)
        {
            os << idea.to_json();
            return os;
        }

        bool operator==(const Content& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                content == other.content &&
                format == other.format &&
                version == other.version &&
                node_id == other.node_id;
        }

        Content() = default;

        Content(const str& content, const int& format, const str& version, const str& node_id)
            : content(content),
              format(static_cast<enums::ContentFormat>(format)),
              version(version),
              node_id(node_id)
        {
        }
    };
}

#endif // CONTENT_H
