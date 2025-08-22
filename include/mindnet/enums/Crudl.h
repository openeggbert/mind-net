///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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

namespace mindnet::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class Crudl
    {
        UNDEFINED = 0, CREATE = 1, READ = 2, UPDATE = 3, DELETE = 4, LIST = 5
    };

    inline std::string crudl_to_string(Crudl difficulty)
    {
        switch (difficulty)
        {
        case Crudl::UNDEFINED:
            return "Undefined";
        case Crudl::CREATE:
            return "Create";
        case Crudl::READ:
            return "Read";
        case Crudl::UPDATE:
            return "Update";
        case Crudl::DELETE:
            return "Delete";
        case Crudl::LIST:
            return "List";
        default:
            return "Unknown";
        }
    }

    inline std::string crudl_to_string(int crudl)
    {
        return crudl_to_string(static_cast<Crudl>(crudl));
    }
} // namespace mindnet::enumss

#endif // CRUDL_H
