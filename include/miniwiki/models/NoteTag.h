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
#ifndef NOTE_TAG_H
#define NOTE_TAG_H

#include <string>
#include "MiniWiki/Helper.h"

namespace miniwiki::Entity {
    using std::string;

    struct NoteTag {
        string note_id;
        int tag_id;

        NoteTag(string note_id_, int tag_id_) : note_id(std::move(note_id_)), tag_id(tag_id_) {
        }

        friend std::ostream &operator<<(std::ostream &os, const NoteTag &noteTag) {
            os << "NoteTag{note_id: " << noteTag.note_id
                    << ", tag_id: " << noteTag.tag_id
                    << "}";
            return os;
        }

        bool operator==(const NoteTag &other) const {
            return this->note_id == other.note_id &&
                   this->tag_id == other.tag_id;
        }
    };
}

#endif // NOTE_TAG_H
