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
#ifndef R2SESSIONCOLUMNS_H
#define R2SESSIONCOLUMNS_H

#include "mindnet/model/BaseColumns.h"

namespace mindnet::plugins::repetition::columns
{
    struct R2SessionColumns : model::BaseColumns
    {
        R2SessionColumns() = delete;

        R2SessionColumns(const R2SessionColumns&) = delete;
        R2SessionColumns& operator=(const R2SessionColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r2_session";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* CLONED_FROM_SESSION_ID = "cloned_from_session_id";

        static constexpr const char* ALGORITHM = "algorithm";
        static constexpr const char* NOTES = "notes";
        static constexpr const char* QUESTIONS = "questions";
        static constexpr const char* SCOPE = "scope";

        static constexpr const char* FILTER_NOTES_UNDER = "filter_notes_under";
        static constexpr const char* FILTER_DATE_FROM = "filter_date_from";
        static constexpr const char* FILTER_DATE_TO = "filter_date_to";
        static constexpr const char* FILTER_TAG = "filter_tag";
        static constexpr const char* FILTER_COLLECTION = "filter_collection";
    };
}
#endif // R2SESSIONCOLUMNS_H
