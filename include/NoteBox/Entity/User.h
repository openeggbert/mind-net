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

#ifndef USER_H
#define USER_H
#include <ostream>
#include <string>

#include "NoteBox/Helper.h"

namespace NoteBox::Entity {
    struct User {
        std::string id;
        std::string nick;
        std::string name;
        std::string surname;
        ll registrationDate;
        ll lastLoginDate;
        bool isActive;

        friend std::ostream &operator<<(std::ostream &os, const User &user) {
            os << "User{id: " << user.id
                    << ", nick: " << user.nick
                    << ", name: " << user.name
                    << ", surname: " << user.surname
                    << ", registrationDate: " << user.registrationDate
                    << ", lastLoginDate: " << user.lastLoginDate
                    << ", isActive: " << (user.isActive ? "true" : "false") << "}";
            return os;
        }

        bool operator==(const User &other) const {
            return
                    this->id == other.id &&
                    this->nick == other.nick &&
                    this->name == other.name &&
                    this->surname == other.surname &&
                    this->registrationDate == other.registrationDate &&
                    this->lastLoginDate == other.lastLoginDate &&
                    this->isActive == other.isActive;
        }
    };
}

#endif
