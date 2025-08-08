///////////////////////////////////////////////////////////////////////////////////////////////
// note-box : Note management tool.
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
#ifndef NOTETABLE_H
#define NOTETABLE_H

namespace NoteBox::Persistence::Impl::Sqlite::Tables {

struct NoteTable {


    NoteTable() = delete;

    NoteTable(const NoteTable&) = delete;
    NoteTable& operator=(const NoteTable&) = delete;

    static constexpr const char* TABLE_NAME = "NOTE";
    
static constexpr const char *ID = "ID";
    static constexpr const char *PARENT_NOTE_ID = "PARENT_NOTE_ID";
    static constexpr const char *TITLE = "TITLE";
    static constexpr const char *CONTENT_ID = "CONTENT_ID";
    static constexpr const char *QUESTION = "QUESTION";
    static constexpr const char *CREATED_AT = "CREATED_AT";
    static constexpr const char *UPDATED_AT = "UPDATED_AT";
    static constexpr const char *LAST_SHOWN_AT = "LAST_SHOWN_AT";
    static constexpr const char *LAST_REVIEWED_AT = "LAST_REVIEWED_AT";
    static constexpr const char *REVIEW_IN_X_DAYS = "REVIEW_IN_X_DAYS";
    static constexpr const char *EXPIRES_AT = "EXPIRES_AT";
    static constexpr const char *IMPORTANCE = "IMPORTANCE";
    static constexpr const char *DIFFICULTY = "DIFFICULTY";
    static constexpr const char *SOURCE_ID = "SOURCE_ID";
};
}

#endif // NOTETABLE_H
