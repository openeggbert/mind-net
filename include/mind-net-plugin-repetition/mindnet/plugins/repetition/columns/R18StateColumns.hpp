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
#ifndef R18STATECOLUMNS_H
#define R18STATECOLUMNS_H

#include "mindnet/model/BaseColumns.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ColumnType.hpp"

namespace mindnet::plugins::repetition::columns
{
    struct R18StateColumns : model::BaseColumns
    {
        R18StateColumns() = delete;

        R18StateColumns(const R18StateColumns&) = delete;
        R18StateColumns& operator=(const R18StateColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r18_state";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* STABILITY_TIMES_100 = "stability_times_100";
        static constexpr const char* LAST_INTERVAL_TIMES_100 = "last_interval_times_100";
        static constexpr const char* REPETITIONS = "repetitions";
        static constexpr const char* LAPSES = "lapses";
        static constexpr const char* NEXT_REVIEW = "next_review";
        static constexpr const char* LAST_REVIEW = "last_review";
        static constexpr const char* LAST_QUALITY = "last_quality";
        static constexpr const char* LAST_SEEN_SEMANTIC_VERSION = "last_seen_semantic_version";
        static constexpr const char* CONTENT_MODIFIED_SINCE_LAST_REVIEW = "content_modified_since_last_review";
    };
}
#endif // R18STATECOLUMNS_H
