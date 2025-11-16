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

namespace mindnet::plugins::core::columns
{
    struct JobEntryColumns : model::BaseColumns
    {
        JobEntryColumns() = delete;

        JobEntryColumns(const JobEntryColumns&) = delete;
        JobEntryColumns& operator=(const JobEntryColumns&) = delete;

        static constexpr const char* MODEL_NAME = "job_entry";

        static constexpr const char* NAME = "name";
        static constexpr const char* DESCRIPTION = "description";
        static constexpr const char* PLUGIN_NAME = "plugin_name";
        static constexpr const char* CRON_EXPRESSION = "cron_expression";
        static constexpr const char* RUN_ONCE_WHEN_MISSED = "run_once_when_missed";
        static constexpr const char* ENABLED = "enabled";
        static constexpr const char* CONFIGURATION = "configuration";
        static constexpr const char* LAST_RUN = "last_run";
        static constexpr const char* NEXT_RUN = "next_run";
    };
}
