///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef SUGGESTIONREVIEWCOLUMNS_H
#define SUGGESTIONREVIEWCOLUMNS_H

#include "../../../model/BaseColumns.h"

namespace mindnet::plugins::suggestion::columns
{
    struct SuggestionReviewColumns : model::BaseColumns
    {
        SuggestionReviewColumns() = delete;

        SuggestionReviewColumns(const SuggestionReviewColumns&) = delete;
        SuggestionReviewColumns& operator=(const SuggestionReviewColumns&) = delete;

        static constexpr const char* MODEL_NAME = "suggestion_review";

        static constexpr const char* SUGGESTION_ID = "suggestion_id";
        static constexpr const char* REVIEWER_ID = "reviewer_id";
        static constexpr const char* DECISION_STATUS = "decision_status";
        static constexpr const char* COMMENT = "comment";
        static constexpr const char* REVIEWED_AT = "reviewed_at";
    };
}
#endif // SUGGESTIONREVIEWCOLUMNS_H
