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
#ifndef REVIEWCOLUMNS_H
#define REVIEWCOLUMNS_H

#include "../../../model/BaseColumns.h"

namespace mindnet::plugins::supermemo::columns
{
    struct ReviewColumns : model::BaseColumns
    {
        ReviewColumns() = delete;

        ReviewColumns(const ReviewColumns&) = delete;
        ReviewColumns& operator=(const ReviewColumns&) = delete;

        static constexpr const char* MODEL_NAME = "review";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* QUESTION_ID = "question_id";
        static constexpr const char* REVIEW_DATE = "review_date";
        static constexpr const char* GRADE = "grade";
        static constexpr const char* RESPONSE_DATA = "response_data";
        static constexpr const char* NOTES = "notes";
    };
}
#endif // REVIEWCOLUMNS_H
