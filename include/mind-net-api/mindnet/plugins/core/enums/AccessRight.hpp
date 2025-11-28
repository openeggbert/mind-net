/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <string>

#include "SingleRight.hpp"

namespace mindnet::plugins::core::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class AccessRight
    {
        // read=4 write=2 delete=1
        None = 0,
        Delete = 1,
        Write = 2,
        WriteDelete = 3,
        Read = 4,
        ReadDelete = 5,
        ReadWrite = 6,
        ReadWriteDelete = 7,
    };

    inline std::string access_right_to_string(AccessRight access_right)
    {
        switch (access_right)
        {
        case AccessRight::None:
            return "None";
        case AccessRight::Delete:
            return "Delete";
        case AccessRight::Write:
            return "Write";
        case AccessRight::WriteDelete:
            return "Write+Delete";
        case AccessRight::Read:
            return "Read";
        case AccessRight::ReadDelete:
            return "Read+Delete";
        case AccessRight::ReadWrite:
            return "Read+Write";
        case AccessRight::ReadWriteDelete:
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
        return access_right == AccessRight::Read || access_right == AccessRight::ReadDelete || access_right ==
            AccessRight::ReadWrite || access_right == AccessRight::ReadWriteDelete;
    }

    inline bool can_write(AccessRight access_right)
    {
        return access_right == AccessRight::Write || access_right == AccessRight::WriteDelete || access_right ==
            AccessRight::ReadWrite || access_right == AccessRight::ReadWriteDelete;
    }

    inline bool can_delete(AccessRight access_right)
    {
        return access_right == AccessRight::Delete || access_right == AccessRight::WriteDelete || access_right ==
            AccessRight::ReadWrite || access_right == AccessRight::ReadWriteDelete;
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
        case SingleRight::None: return false;
        case SingleRight::Read: return can_read(access_right);
        case SingleRight::Write: return can_write(access_right);
        case SingleRight::Delete: return can_delete(access_right);
        default: return false;
        }
    }
} // namespace mindnet::enums