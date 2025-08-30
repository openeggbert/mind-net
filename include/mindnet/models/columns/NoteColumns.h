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
#ifndef NOTECOLUMNS_H
#define NOTECOLUMNS_H


#include "BaseColumns.h"


namespace mindnet::models::columns
{
    struct NoteColumns : BaseColumns
    {
        NoteColumns() = delete;

        NoteColumns(const NoteColumns&) = delete;
        NoteColumns& operator=(const NoteColumns&) = delete;

        static constexpr const char* MODEL_NAME = "note";

        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* TITLE = "title";
        static constexpr const char* PARENT_NOTE_ID = "parent_note_id";
        static constexpr const char* CONTENT_ID = "content_id";
        static constexpr const char* SIBLING_POSITION = "sibling_position";
        static constexpr const char* IMPORTANCE = "importance";
        static constexpr const char* DIFFICULTY = "difficulty";
    };
}
#endif // NOTECOLUMNS_H