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
#ifndef IDEA_H
#define IDEA_H

#include <ostream>
#include <string>
#include "NoteBox/Helper.h"
#include "NoteBox/Utils.h"

namespace NoteBox::Entity
{
    using std::string;

    struct Idea
    {
        int id;
        string text;
        string category;
        unixtime created_at;
        unixtime due_at;

        friend std::ostream& operator<<(std::ostream& os, const Idea& idea)
        {
            os << "Idea{id: " << idea.id
                << ", text: " << idea.text
                << ", category: " << idea.category
                << ", created_at: " << Utils::unixToFormattedString(idea.created_at)
                << ", due_at: " << Utils::unixToFormattedString(idea.due_at)
                << "}";
            return os;
        }

        bool operator==(const Idea& other) const
        {
            return
                this->id == other.id &&
                this->text == other.text &&
                this->category == other.category &&
                this->created_at == other.created_at &&
                this->due_at == other.due_at;
        }
    };
}
#endif // IDEA_H
