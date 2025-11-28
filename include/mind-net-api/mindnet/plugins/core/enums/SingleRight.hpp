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

#include "mindnet/model/EnumDefinition.hpp"

namespace mindnet::plugins::core::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class SingleRight
    {
        None = 0,
        Read = 4,
        Write = 2,
        Delete = 1
    };

    inline std::string single_right_to_string(SingleRight single_right)
    {
        switch (single_right)
        {
        case SingleRight::None:
            return "None";
        case SingleRight::Delete:
            return "Delete";
        case SingleRight::Write:
            return "Write";
        case SingleRight::Read:
            return "Read";
        default:
            return "Unknown";
        }
    }

    inline std::string single_right_to_string(int single_right)
    {
        return single_right_to_string(static_cast<SingleRight>(single_right));
    }

    inline mindnet::model::EnumDefinition single_right_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            single_right_to_string, 4, 0, 4, 2, 1
        };
    }
} // namespace mindnet::enums