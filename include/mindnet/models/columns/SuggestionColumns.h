///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef SUGGESTIONCOLUMNS_H
#define SUGGESTIONCOLUMNS_H


#include "BaseColumns.h"


namespace mindnet::models::columns
{
    struct SuggestionColumns : BaseColumns
    {
        SuggestionColumns() = delete;

        SuggestionColumns(const SuggestionColumns&) = delete;
        SuggestionColumns& operator=(const SuggestionColumns&) = delete;

        static constexpr const char* MODEL_NAME = "suggestion";

        static constexpr const char* PARENT_SUGGESTION_ID = "parent_suggestion_id";
        static constexpr const char* FROM_USER_ID = "from_user_id";
        static constexpr const char* TABLE_NAME = "table_name";
        static constexpr const char* OPERATION = "operation";
        static constexpr const char* STATUS = "status";
        static constexpr const char* DATA_JSON = "data_json";
        static constexpr const char* REVIEW_COUNT = "review_count";
    };
}
#endif // SUGGESTIONCOLUMNS_H