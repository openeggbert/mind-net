///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
// or write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Column definitions for the r_review table which stores review records
 * for spaced repetition learning.
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef RREVIEWCOLUMNS_H
#define RREVIEWCOLUMNS_H

#include "mindnet/model/BaseColumns.hpp"

namespace mindnet::plugins::repetition::columns
{
    struct RReviewColumns : model::BaseColumns
    {
        RReviewColumns() = delete;
        RReviewColumns(const RReviewColumns&) = delete;
        RReviewColumns& operator=(const RReviewColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r_review";

        // Core fields
        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* R_SESSION_ID = "r_session_id";
        static constexpr const char* ALGORITHM = "algorithm";

        // Review target (must have note_id)
        static constexpr const char* NOTE_ID = "note_id";

        // Review data
        static constexpr const char* REVIEW_DATE = "review_date";
        static constexpr const char* GRADE = "grade"; // 0-5
        static constexpr const char* RESPONSE_DATA = "response_data";
        static constexpr const char* NOTES = "notes";

        // Timing metrics
        static constexpr const char* STARTED_AT = "started_at";
        static constexpr const char* ENDED_AT = "ended_at";
        static constexpr const char* LATENCY_MS = "latency_ms";

        // User behavior
        static constexpr const char* ANSWER_CHANGE_COUNT = "answer_change_count";

        // Algorithm-specific data
        static constexpr const char* DETAILS_JSON = "details_json";
    };
}
#endif // RREVIEWCOLUMNS_H
