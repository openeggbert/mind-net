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
#ifndef CONTENT_H
#define CONTENT_H

#include <ostream>
#include <string>
#include "miniwiki/Helper.h"

namespace miniwiki::Entity
{
    using std::string;

    struct Content
    {
        string id;
        string value;

        Content() = default;
        Content(string& id_, string& value_) :
            id(std::move(id_)),
            value(std::move(value_))
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const Content& content)
        {
            os << "Content{id: " << content.id
                << ", value: " << content.value
                << "}";
            return os;
        }

        bool operator==(const Content& other) const
        {
            return
                this->id == other.id &&
                this->value == other.value;
        }
    };
}

#endif // CONTENT_H
