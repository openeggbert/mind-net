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
#ifndef LITERATURESOURCE_H
#define LITERATURESOURCE_H


#include <ostream>
#include <string>
#include "NoteBox/Helper.h"

namespace NoteBox::Enums {
using std::string;
    struct Note {
        string id;
        string title;
        string content;

        // std::string subject;
        // std::string topic;
        // std::string subtopic;
        // std::string category;
        // std::string question;
        // std::string answer;
        // QuestionDifficulty difficulty;
        // QuestionImportance importance;
        // ll last_update;

        Note(string id_, string title_, string content_) :
        id(std::move(id_)), title(std::move(title_)), content(std::move(content_)) {}

        friend std::ostream& operator<<(std::ostream& os, const Note& file) {
            os << "Question{id: " << file.id
               // << ", subject: " << file.subject
               // << ", topic: " << file.topic
               // << ", subtopic: " << file.subtopic
               // << ", category: " << file.category
               // << ", question: " << file.question
               // << ", answer: " << file.answer
               // << ", difficulty: " << static_cast<int>(file.difficulty)
               // << ", importance: " << static_cast<int>(file.importance)
               // << ", last_update: " << file.last_update
               << "}";
            return os;
        }

        bool operator==(const Note &other) const {
            return
                    this->id == other.id;
                    // this->subject == other.subject &&
                    // this->topic == other.topic &&
                    // this->subtopic == other.subtopic &&
                    // this->category == other.category &&
                    // this->question == other.question &&
                    // this->answer == other.answer &&
                    // this->difficulty == other.difficulty &&
                    // this->importance == other.importance &&
                    // this->last_update == other.last_update;
        }
    };

}

#endif
#endif // LITERATURESOURCE_H
