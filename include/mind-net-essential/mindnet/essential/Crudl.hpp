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


namespace mindnet::essential
{
    /**
     *
     * @author robertvokac
     */
    enum class Crudl : unsigned char
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

