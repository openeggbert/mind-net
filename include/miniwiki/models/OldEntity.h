///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Note management tool.
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
#ifndef OLDENTITY_H
#define OLDENTITY_H


#include "ostream"
#include <string>

#include "miniwiki/Helper.h"

namespace miniwiki::Entity
{
    using std::string;

    struct OldEntity
    {
        string id;
        string entity_name;
        string entity_id;
        string value;
        unixtime added_at = 0ll;

        OldEntity() = default;


        friend std::ostream& operator<<(std::ostream& os, const OldEntity& content)
        {
            os << "Content{id: " << content.id
                << ", entity_name: " << content.entity_name
                << ", entity_id: " << content.entity_id
                << ", value: " << content.value
                << ", added: " << content.added_at
                << "}";
            return os;
        }

        bool operator==(const OldEntity& other) const
        {
            return
                this->id == other.id &&
                this->entity_name == other.entity_name &&
                this->entity_id == other.entity_id &&
                this->value == other.value &&
                this->added_at == other.added_at;
        }
    };
}

#endif // OLDENTITY_H
