///////////////////////////////////////////////////////////////////////////////////////////////
// brain-meter: Tool that asks the user questions.
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

#ifndef REVIEW_H
#define REVIEW_H
#include <ostream>
#include <string>
#include "../Enums/QuestionDifficulty.h"

namespace NoteBox::Entity {
            struct Review {
        int id;
        std::string subject;
        std::string topic;
        std::string subtopic;
        std::string category;
        std::string question;
        std::string answer;
        Enums::QuestionDifficulty difficulty;
        std::string lastCheckResult;

        friend std::ostream& operator<<(std::ostream& os, const Review& review) {
            os << "Review{id: " << review.id
               << ", subject: " << review.subject
               << ", topic: " << review.topic
               << ", subtopic: " << review.subtopic
               << ", category: " << review.category
               << ", question: " << review.question
               << ", answer: " << review.answer
               << ", difficulty: " << static_cast<int>(review.difficulty)
               << ", lastCheckResult: " << review.lastCheckResult << "}";
            return os;
        }

        bool operator==(const Review &other) const {
            return
                    this->id == other.id &&
                    this->subject == other.subject &&
                    this->topic == other.topic &&
                    this->subtopic == other.subtopic &&
                    this->category == other.category &&
                    this->question == other.question &&
                    this->answer == other.answer &&
                    this->difficulty == other.difficulty &&
                    this->lastCheckResult == other.lastCheckResult;
        }
    };


}

#endif