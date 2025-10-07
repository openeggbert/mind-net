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
#ifndef R2REVIEWCOLUMNS_H
#define R2REVIEWCOLUMNS_H


#include "mindnet/model/BaseColumns.h"

namespace mindnet::plugins::repetition::columns
{
    struct R2ReviewColumns : model::BaseColumns
    {
        R2ReviewColumns() = delete;

        R2ReviewColumns(const R2ReviewColumns&) = delete;
        R2ReviewColumns& operator=(const R2ReviewColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r2_review";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* QUESTION_ID = "question_id";
        static constexpr const char* REVIEW_DATE = "review_date";
        static constexpr const char* GRADE = "grade";
        static constexpr const char* RESPONSE_DATA = "response_data";
        static constexpr const char* NOTES = "notes";
    };
}
#endif // R2REVIEWCOLUMNS_H
