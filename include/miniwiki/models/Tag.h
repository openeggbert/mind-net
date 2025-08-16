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
#ifndef TAG_H
#define TAG_H

#include <ostream>
#include <string>
#include "MiniWiki/Helper.h"

namespace miniwiki::Entity {
    using std::string;

    struct Tag {
        int id;
        string name;

        Tag(int id_, string name_) : id(id_), name(std::move(name_)) {
        }

        friend std::ostream &operator<<(std::ostream &os, const Tag &tag) {
            os << "Tag{id: " << tag.id
                    << ", name: " << tag.name
                    << "}";
            return os;
        }

        bool operator==(const Tag &other) const {
            return this->id == other.id &&
                   this->name == other.name;
        }
    };
}

#endif // TAG_H
