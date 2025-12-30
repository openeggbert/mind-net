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
    enum class VisitSource
    {
        Unknown          = 0,
        Created          = 1,
        Search           = 2,
        AdvancedSearch   = 3,
        Link             = 4,
        Backlink         = 5,

        Random           = 10,
        Repetition       = 11,
        VisitedHistory   = 12,
        LastVisited      = 13,

        Older            = 20,
        Newer            = 21,

        Other            = 50,
        External         = 100,
    };

    inline std::string visit_source_to_string(const VisitSource source)
    {
        switch (source)
        {
        case VisitSource::Unknown:         return "Unknown";
        case VisitSource::Created:         return "Created";
        case VisitSource::Search:          return "Search";
        case VisitSource::AdvancedSearch:  return "AdvancedSearch";
        case VisitSource::Link:            return "Link";
        case VisitSource::Backlink:        return "Backlink";

        case VisitSource::Random:          return "Random";
        case VisitSource::Repetition:      return "Repetition";
        case VisitSource::VisitedHistory:  return "VisitedHistory";
        case VisitSource::LastVisited:     return "LastVisited";

        case VisitSource::Older:           return "Older";
        case VisitSource::Newer:           return "Newer";

        case VisitSource::Other:           return "Other";
        case VisitSource::External:        return "External";
        default:
            return "Unknown";
        }
    }

    inline std::string visit_source_to_string(int source)
    {
        return visit_source_to_string(static_cast<VisitSource>(source));
    }

    inline mindnet::model::EnumDefinition visit_source_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            visit_source_to_string,
            14, // number of enum values
            0, 1, 2, 3, 4, 5,
            10, 11, 12, 13,
            20, 21,
            50, 100
        };
    }
}
