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
#ifndef REFERENCE_H
#define REFERENCE_H

#include <ostream>
#include <string>
#include "MiniWiki/Helper.h"

namespace miniwiki::Entity {
    using std::string;

    struct Reference {
        string from_note_id;
        string to_note_id;
        string link_type;

        Reference(string from_note_id_, string to_note_id_, string link_type_ = "related") : from_note_id(
                std::move(from_note_id_)),
            to_note_id(std::move(to_note_id_)),
            link_type(std::move(link_type_)) {
        }

        friend std::ostream &operator<<(std::ostream &os, const Reference &reference) {
            os << "Reference{from_note_id: " << reference.from_note_id
                    << ", to_note_id: " << reference.to_note_id
                    << ", link_type: " << reference.link_type
                    << "}";
            return os;
        }

        bool operator==(const Reference &other) const {
            return this->from_note_id == other.from_note_id &&
                   this->to_note_id == other.to_note_id &&
                   this->link_type == other.link_type;
        }
    };
}

#endif // REFERENCE_H
