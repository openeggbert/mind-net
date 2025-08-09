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
#ifndef OLDCONTENT_H
#define OLDCONTENT_H


#include <ostream>
#include <string>

#include "NoteBox/Helper.h"

namespace NoteBox::Entity
{
    using std::string;

    struct OldContent
    {
        string id;
        string value;
        unixtime added_at = 0ll;

        OldContent() = default;


        friend std::ostream& operator<<(std::ostream& os, const OldContent& content)
        {
            os << "Content{id: " << content.id
                << ", value: " << content.value
                << ", added: " << content.added_at
                << "}";
            return os;
        }

        bool operator==(const OldContent& other) const
        {
            return
                this->id == other.id &&
                this->value == other.value &&
                this->added_at == other.added_at;
        }
    };
}
#endif // OLDCONTENT_H
