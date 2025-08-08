///////////////////////////////////////////////////////////////////////////////////////////////
// note-box : Note management tool.
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
#include "NoteBox/Helper.h"

namespace NoteBox::Entity {
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
        unixtime last_reviewed_at;
        unixtime expires_at;
        int review_in_x_days;
        int importance;
        int difficulty;
        int source_id;

        // Note();
        //
        // Note(string& id_, string& title_, const string& content_) : id(std::move(id_)), title(std::move(title_)),
        //                                                    content_id(std::move(content_id)),
        //                                                    review_in_x_days(0), importance(0),
        //                                                    difficulty(0), source_id(0) {
        // }

        friend std::ostream &operator<<(std::ostream &os, const Note &note) {
            os << "Note{id: " << note.id
                    << ", parent_note_id: " << note.parent_note_id
                    << ", title: " << note.title
                    << ", content_id: " << note.content_id
                    << ", question: " << note.question
                    << ", created_at: " << note.created_at
                    << ", updated_at: " << note.updated_at
                    << ", last_shown_at: " << note.last_shown_at
                    << ", last_reviewed_at: " << note.last_reviewed_at
                    << ", review_in_x_days: " << note.review_in_x_days
                    << ", expires_at: " << note.expires_at
                    << ", importance: " << note.importance
                    << ", difficulty: " << note.difficulty
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
