///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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
#ifndef CRUDL_H
#define CRUDL_H

#include <string>

namespace mindnet::essential
{
    /**
     *
     * @author robertvokac
     */
    enum class Crudl
    {
        Undefined = 0, Create = 1, Read = 2, Update = 3, Delete = 4, List = 5
    };
    inline constexpr int CRUDL_VALUE_COUNT = 6;

    inline std::string crudl_to_string(Crudl difficulty)
    {
        switch (difficulty)
        {
        case Crudl::Undefined:
            return "Undefined";
        case Crudl::Create:
            return "Create";
        case Crudl::Read:
            return "Read";
        case Crudl::Update:
            return "Update";
        case Crudl::Delete:
            return "Delete";
        case Crudl::List:
            return "List";
        default:
            return "Unknown";
        }
    }

    inline std::string crudl_to_string(int crudl)
    {
        return crudl_to_string(static_cast<Crudl>(crudl));
    }

    inline Crudl char_to_crudl(const char ch)
    {
        char lowercase_ch = std::tolower(ch);
        switch (lowercase_ch)
        {
        case 'c':
            return Crudl::Create;
        case 'r':
            return Crudl::Read;
        case 'u':
            return Crudl::Update;
        case 'd':
            return Crudl::Delete;
        case 'l':
            return Crudl::List;
        default:
            return Crudl::Undefined;
        }
    }

    static const constexpr char* CRUDL = "CRUDL";
}

#endif // CRUDL_H
