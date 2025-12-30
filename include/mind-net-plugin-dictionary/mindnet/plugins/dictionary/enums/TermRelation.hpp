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

namespace mindnet::plugins::dictionary::enums
{
    enum class TermRelationType
    {
        None = 0,

        IsA = 1,
        PartOf = 2,
        Uses = 3,
        DependsOn = 4,
        Implements = 5,
        Related = 6,
        Contrasts = 7,
        AlternativeTo = 8
    };

    inline std::string term_relation_type_to_string(TermRelationType type)
    {
        switch (type)
        {
        case TermRelationType::None:
            return "None";

        case TermRelationType::IsA:
            return "Is a";
        case TermRelationType::PartOf:
            return "Part of";
        case TermRelationType::Uses:
            return "Uses";
        case TermRelationType::DependsOn:
            return "Depends on";
        case TermRelationType::Implements:
            return "Implements";
        case TermRelationType::Related:
            return "Related";
        case TermRelationType::Contrasts:
            return "Contrasts";
        case TermRelationType::AlternativeTo:
            return "Alternative to";
        default:
            return "None";
        }
    }

    inline bool is_symmetric(TermRelationType t)
    {
        switch (t)
        {
        case TermRelationType::Related:
        case TermRelationType::Contrasts:
        case TermRelationType::AlternativeTo:
            return true;

        case TermRelationType::None:
        case TermRelationType::IsA:
        case TermRelationType::PartOf:
        case TermRelationType::Uses:
        case TermRelationType::DependsOn:
        case TermRelationType::Implements:
            return false;

        default:
            return false;
        }
    }

    inline bool is_transitive(TermRelationType t)
    {
        switch (t)
        {
        case TermRelationType::IsA:
        case TermRelationType::PartOf:
        case TermRelationType::DependsOn:
            return true;

        case TermRelationType::None:
        case TermRelationType::Uses:
        case TermRelationType::Implements:
        case TermRelationType::Related:
        case TermRelationType::Contrasts:
        case TermRelationType::AlternativeTo:
            return false;

        default:
            return false;
        }
    }

    struct TermRelationTraits
    {
        bool symmetric;
        bool transitive;
    };

    inline TermRelationTraits get_term_relation_traits(TermRelationType t)
    {
        return {
            is_symmetric(t),
            is_transitive(t),
        };
    }


    inline std::string term_relation_type_to_string(int type)
    {
        return term_relation_type_to_string(
            static_cast<TermRelationType>(type));
    }

    inline mindnet::model::EnumDefinition term_relation_type_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            term_relation_type_to_string,
            9,
            0, 1, 2, 3, 4, 5, 6, 7, 8
        };
    }
}
