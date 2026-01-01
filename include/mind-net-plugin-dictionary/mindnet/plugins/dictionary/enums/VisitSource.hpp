#pragma once

#include <string>

#include "mindnet/model/EnumDefinition.hpp"

namespace mindnet::plugins::dictionary::enums
{
    enum class VisitSource
    {
        Unknown        = 0,
        Created        = 1,
        Search         = 2,
        AdvancedSearch = 3,
        Link           = 4,
        Backlink       = 5,

        Random         = 10,
        Repetition     = 11,
        VisitedHistory = 12,
        LastVisited    = 13,
        Refresh        = 14,
        Discard        = 15,

        Older          = 20,
        Newer          = 21,

        Other          = 50,
        External       = 100,
    };

    inline std::string visit_source_to_string(VisitSource source)
    {
        switch (source)
        {
        case VisitSource::Unknown:        return "Unknown";
        case VisitSource::Created:        return "Created";
        case VisitSource::Search:         return "Search";
        case VisitSource::AdvancedSearch: return "AdvancedSearch";
        case VisitSource::Link:           return "Link";
        case VisitSource::Backlink:       return "Backlink";

        case VisitSource::Random:         return "Random";
        case VisitSource::Repetition:     return "Repetition";
        case VisitSource::VisitedHistory: return "VisitedHistory";
        case VisitSource::LastVisited:    return "LastVisited";
        case VisitSource::Refresh:        return "Refresh";
        case VisitSource::Discard:        return "Discard";

        case VisitSource::Older:          return "Older";
        case VisitSource::Newer:          return "Newer";

        case VisitSource::Other:          return "Other";
        case VisitSource::External:       return "External";
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
            17, // number of enum values
            0, 1, 2, 3, 4, 5,
            10, 11, 12, 13, 14, 15,
            20, 21,
            50, 100
        };
    }
}
