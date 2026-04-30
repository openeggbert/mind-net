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

#include "hive/model/EnumDefinition.hpp"

namespace hive::plugins::slipbox::enums
{
    enum class QuestionType
    {
        Single = 0,
        Multi = 1,
        TrueFalse = 2,
        Text = 3
    };

    inline std::string question_type_to_string(const QuestionType type)
    {
        switch (type)
        {
        case QuestionType::Single:
            return "Single";
        case QuestionType::Multi:
            return "Multi";
        case QuestionType::TrueFalse:
            return "TrueFalse";
        case QuestionType::Text:
            return "Text";
        default:
            return "Unknown";
        }
    }

    inline std::string question_type_to_string(int type)
    {
        return question_type_to_string(static_cast<QuestionType>(type));
    }

    inline hive::model::EnumDefinition question_type_to_enum_definition()
    {
        return hive::model::EnumDefinition{
            question_type_to_string, 4, 0, 1, 2, 3
        };
    }
} // namespace hive::enums