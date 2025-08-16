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
#ifndef HISTORY_H
#define HISTORY_H

#include <ostream>
#include <string>
#include "MiniWiki/Helper.h"

namespace MiniWiki::Entity {
    using std::string;

    struct History {
        string id;
        string note_id;
        string action;
        string timestamp;

        History(string id_, string note_id_, string action_, string timestamp_) : id(std::move(id_)),
            note_id(std::move(note_id_)),
            action(std::move(action_)),
            timestamp(std::move(timestamp_)) {
        }

        friend std::ostream &operator<<(std::ostream &os, const History &history) {
            os << "History{id: " << history.id
                    << ", note_id: " << history.note_id
                    << ", action: " << history.action
                    << ", timestamp: " << history.timestamp
                    << "}";
            return os;
        }

        bool operator==(const History &other) const {
            return this->id == other.id &&
                   this->note_id == other.note_id &&
                   this->action == other.action &&
                   this->timestamp == other.timestamp;
        }
    };
}

#endif // HISTORY_H
