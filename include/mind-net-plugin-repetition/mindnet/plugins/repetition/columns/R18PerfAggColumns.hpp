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
#ifndef R18PERFAGGCOLUMNS_H
#define R18PERFAGGCOLUMNS_H

#include "mindnet/model/BaseColumns.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ColumnType.hpp"

namespace mindnet::plugins::repetition::columns
{
    struct R18PerfAggColumns : model::BaseColumns
    {
        R18PerfAggColumns() = delete;

        R18PerfAggColumns(const R18PerfAggColumns&) = delete;
        R18PerfAggColumns& operator=(const R18PerfAggColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r18_perf_agg";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* BIN_LOG_T_TIMES_100 = "bin_log_t_times_100";
        static constexpr const char* TOTAL = "total";
        static constexpr const char* CORRECT = "correct";
    };
}
#endif // R18PERFAGGCOLUMNS_H
