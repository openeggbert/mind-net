///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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

#include "mindnet/model/BaseModel.h"

namespace mindnet::model
{
    [[nodiscard]] JSON model_to_json(
        entity_fields fields,
        const ModelDefinition& definition)
    {
        JSON json;
        int index = 0;

        for (auto& e : definition.get_columns())
        {
            auto field = e.get_column_name();
            std::visit([&json, &field](const auto& value)
            {
                json[field] = value;
            }, fields[index]);
            index++;
        }
        return json;
    }
}
