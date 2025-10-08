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
#ifndef R4STATECOLUMNS_H
#define R4STATECOLUMNS_H


#include "mindnet/model/BaseColumns.h"
#include "mindnet/essential/Helper.h"
#include "mindnet/model/ColumnType.h"

namespace mindnet::plugins::repetition::columns
{
    struct R4StateColumns : model::BaseColumns
    {
        R4StateColumns() = delete;

        R4StateColumns(const R4StateColumns&) = delete;
        R4StateColumns& operator=(const R4StateColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r4_state";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* QUESTION_ID = "question_id";
        static constexpr const char* REPETITIONS = "repetitions";
        static constexpr const char* INTERVAL = "interval";
        static constexpr const char* EF_TIMES_100 = "ef_times_100";
        static constexpr const char* CORRECTION_FACTOR_TIMES_100 = "correction_factor_times_100";
        static constexpr const char* NEXT_REVIEW = "next_review";
        static constexpr const char* LAST_REVIEW = "last_review";
        static constexpr const char* LAST_QUALITY = "last_quality";
    };
}
#endif // R4STATECOLUMNS_H
