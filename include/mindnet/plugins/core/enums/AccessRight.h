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
#ifndef ACCESSRIGHT_H
#define ACCESSRIGHT_H

#include <string>

#include "SingleRight.h"

namespace mindnet::plugins::core::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class AccessRight
    {
        // read=4 write=2 delete=1
        NONE = 0,
        DELETE = 1,
        WRITE = 2,
        WRITE_DELETE = 3,
        READ = 4,
        READ_DELETE = 5,
        READ_WRITE = 6,
        READ_WRITE_DELETE = 7,
    };

    inline std::string access_right_to_string(AccessRight access_right)
    {
        switch (access_right)
        {
        case AccessRight::NONE:
            return "None";
        case AccessRight::DELETE:
            return "Delete";
        case AccessRight::WRITE:
            return "Write";
        case AccessRight::WRITE_DELETE:
            return "Write+Delete";
        case AccessRight::READ:
            return "Read";
        case AccessRight::READ_DELETE:
            return "Read+Delete";
        case AccessRight::READ_WRITE:
            return "Read+Write";
        case AccessRight::READ_WRITE_DELETE:
            return "Read+Write+Delete";
        default:
            return "Unknown";
        }
    }

    inline std::string access_right_to_string(int access_right)
    {
        return access_right_to_string(static_cast<AccessRight>(access_right));
    }

    inline mindnet::model::EnumDefinition access_right_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            access_right_to_string, 8, 0, 1, 2, 3, 4, 5, 6, 7
        };
    }

    inline bool can_read(AccessRight access_right)
    {
        return access_right == AccessRight::READ || access_right == AccessRight::READ_DELETE || access_right ==
            AccessRight::READ_WRITE || access_right == AccessRight::READ_WRITE_DELETE;
    }

    inline bool can_write(AccessRight access_right)
    {
        return access_right == AccessRight::WRITE || access_right == AccessRight::WRITE_DELETE || access_right ==
            AccessRight::READ_WRITE || access_right == AccessRight::READ_WRITE_DELETE;
    }

    inline bool can_delete(AccessRight access_right)
    {
        return access_right == AccessRight::DELETE || access_right == AccessRight::WRITE_DELETE || access_right ==
            AccessRight::READ_WRITE || access_right == AccessRight::READ_WRITE_DELETE;
    }

    inline bool can_read(int access_right)
    {
        return can_read(static_cast<AccessRight>(access_right));
    }

    inline bool can_write(int access_right)
    {
        return can_write(static_cast<AccessRight>(access_right));
    }

    inline bool can_delete(int access_right)
    {
        return can_delete(static_cast<AccessRight>(access_right));
    }

    inline bool can(SingleRight single_right, int access_right)
    {
        switch (single_right)
        {
        case SingleRight::NONE: return false;
        case SingleRight::READ: return can_read(access_right);
        case SingleRight::WRITE: return can_write(access_right);
        case SingleRight::DELETE: return can_delete(access_right);
        default: return false;
        }
    }
} // namespace mindnet::enums
#endif // ACCESSRIGHT_H
