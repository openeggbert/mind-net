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
#ifndef SESSION_H
#define SESSION_H


#include "ostream"
#include <string>

#include "MiniWiki/Helper.h"
#include "MiniWiki/Utils.h"

namespace MiniWiki::Entity
{
    using std::string;

    struct Session
    {
        int id;
        string current_path;
        string editor_path;
        unixtime last_opened;

        Session(int id_, string current_path_,
            const string& editor_path_, unixtime last_opened_) : id(id_),
                                                                      current_path(std::move(current_path_)),
        editor_path(editor_path_),
                                                                      last_opened(last_opened_)
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const Session& session)
        {
            os << "Session{id: " << session.id
                << ", current_path: " << session.current_path
                << ", editor_path: " << session.editor_path
                << ", last_opened: " << Utils::unixToFormattedString(session.last_opened)
                << "}";
            return os;
        }

        bool operator==(const Session& other) const
        {
            return this->id == other.id &&
                this->current_path == other.current_path &&
                this->editor_path == other.editor_path &&
                this->last_opened == other.last_opened;
        }
    };
}
#endif // SESSION_H
