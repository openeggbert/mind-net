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

// unknown
//   -- I know the term exists, but nothing more.
//
// recognized
//   -- I recognize the name and its general context.
//
// understood
//   -- I understand the definition and core idea.
//
// applied
//   -- I can correctly use it in practice.
//
// internalized
//   -- Using it is automatic and requires no conscious effort.

namespace mindnet::plugins::dictionary::enums
{
    enum class UnderstandingLevel
    {
        Unknown = 0,
        Recognized = 1,
        Understood = 2,
        Applied = 3,
        Internalized = 4,
    };

    inline std::string understanding_level_to_string(const UnderstandingLevel level)
    {
        switch (level)
        {
        case UnderstandingLevel::Unknown:
            return "Unknown";
        case UnderstandingLevel::Recognized:
            return "Recognized";
        case UnderstandingLevel::Understood:
            return "Understood";
        case UnderstandingLevel::Applied:
            return "Applied";
        case UnderstandingLevel::Internalized:
            return "Internalized";
        default:
            return "Unknown";
        }
    }

    inline std::string understanding_level_to_string(int level)
    {
        return understanding_level_to_string(static_cast<UnderstandingLevel>(level));
    }

    inline mindnet::model::EnumDefinition understanding_level_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            understanding_level_to_string, 5, 0, 1, 2, 3, 4
        };
    }
}