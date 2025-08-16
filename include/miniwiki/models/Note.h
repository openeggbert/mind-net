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
#ifndef NOTE_H
#define NOTE_H


#include <ostream>
#include <string>
#include "miniwiki/Helper.h"
#include "miniwiki/Utils.h"
#include "miniwiki/Enum/Difficulty.h"
#include "miniwiki/Enum/Importance.h"

namespace MiniWiki::Entity {
    using std::string;

    struct Note {
        string id;
        string parent_note_id;
        string title;
        string content_id;
        string question;
        unixtime created_at;
        unixtime updated_at;
        unixtime last_shown_at;

        unixtime expires_at;
        unixtime last_reviewed_at;
        unsigned short review_in_x_days;
        unsigned char importance;
        unsigned char difficulty;
        unsigned short source_id;
//         New columns:
//         bool can_be_updated//default true
//         bool can_be deleted//default false
//         bool system
//         bool archived
//         bool concept

        [[nodiscard]] std::string get_last_id_segment() const
        {
            if (id.empty())
            {
                return id;
            }
            auto vector = Utils::note_id_to_vector(id);
            return vector.back();

        }
        friend std::ostream &operator<<(std::ostream &os, const Note &note) {
            os << "Note{id: " << note.id
                    << ", parent_note_id: " << note.parent_note_id
                    << ", title: " << note.title
                    << ", content_id: " << note.content_id
                    << ", question: " << note.question
                    << ", created_at: " << Utils::unixToFormattedString(note.created_at)
                    << ", updated_at: " << Utils::unixToFormattedString(note.updated_at)
                    << ", last_shown_at: " << Utils::unixToFormattedString(note.last_shown_at)
                    << ", last_reviewed_at: " << Utils::unixToFormattedString(note.last_reviewed_at)
                    << ", expires_at: " << Utils::unixToFormattedString(note.expires_at)
                    << ", review_in_x_days: " << note.review_in_x_days
                    << ", importance: " << Enum::importance_to_string(note.importance)
                    << ", difficulty: " << Enum::difficulty_to_string(note.difficulty)
                    << ", source_id: " << note.source_id
                    << "}";
            return os;
        }

        bool operator==(const Note &other) const {
            return
                    this->id == other.id &&
                    this->parent_note_id == other.parent_note_id &&
                    this->title == other.title &&
                    this->content_id == other.content_id &&
                    this->question == other.question &&
                    this->created_at == other.created_at &&
                    this->updated_at == other.updated_at &&
                    this->last_shown_at == other.last_shown_at &&
                    this->last_reviewed_at == other.last_reviewed_at &&
                    this->review_in_x_days == other.review_in_x_days &&
                    this->expires_at == other.expires_at &&
                    this->importance == other.importance &&
                    this->difficulty == other.difficulty &&
                    this->source_id == other.source_id;
        }
    };
}

#endif // NOTE_H
