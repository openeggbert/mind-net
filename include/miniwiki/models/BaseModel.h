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
#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <nlohmann/json.hpp>

#include <string>

#include "miniwiki/Helper.h"

namespace miniwiki::models {

    using std::string;

    struct BaseModel {

        virtual ~BaseModel() = default;
        [[nodiscard]] virtual str get_entity_name() const = 0;
        [[nodiscard]] virtual entity_columns get_entity_columns() const = 0;
        [[nodiscard]] virtual entity_fields get_entity_fields() const = 0;
        [[nodiscard]] virtual bool should_be_id_auto_incremented() const = 0;

        [[nodiscard]] JSON to_json() const
        {
            JSON json;
            int index = 0;
            entity_fields fields = get_entity_fields();
            for (auto& field : get_entity_columns())
            {
                std::visit([&json, &field](const auto& value)
                {
                    json[field] = value;
                }, fields[index]);
                index++;
            }
            return json;
        };

        // [[nodiscard]] bool equals(const BaseModel &other) const
        // {
        //     return to_json() == other.to_json();
        // };
        void print(std::ostream &os) const
        {
            os << to_json();
        };
    };
}

#endif // BASEMODEL_H
