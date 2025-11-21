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
#pragma once


#include "mindnet/model/BaseColumns.hpp"

namespace mindnet::plugins::slipbox::columns
{
    struct TestColumns : model::BaseColumns
    {
        TestColumns() = delete;

        TestColumns(const TestColumns&) = delete;
        TestColumns& operator=(const TestColumns&) = delete;

        static constexpr const char* MODEL_NAME = "test";

        static constexpr const char* UNDER_NOTE_ID = "under_note_id";
        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* TITLE = "title";
        static constexpr const char* DESCRIPTION = "description";

        static constexpr const char* TIME_LIMIT_IN_SECONDS = "time_limit_in_seconds";
        static constexpr const char* ANSWER_COUNT_LIMIT = "answer_count_limit";
        static constexpr const char* ATTEMPT_LIMIT = "attempt_limit";
        static constexpr const char* IS_PUBLIC = "is_public";
    };
}
