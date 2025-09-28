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
#ifndef NOTECOLUMNS_H
#define NOTECOLUMNS_H

#include "mindnet/model/BaseColumns.h"

namespace mindnet::plugins::slipbox::columns
{
    struct NoteColumns : model::BaseColumns
    {
        NoteColumns() = delete;

        NoteColumns(const NoteColumns&) = delete;
        NoteColumns& operator=(const NoteColumns&) = delete;

        static constexpr const char* MODEL_NAME = "note";

        static constexpr const char* MAP_ID = "map_id";
        static constexpr const char* PARENT_NOTE_ID = "parent_note_id";
        static constexpr const char* CONTENT_ID = "content_id";
        static constexpr const char* SOURCE_ID = "source_id";
        static constexpr const char* ALIAS_FOR_NOTE_ID = "alias_for_note_id";
        static constexpr const char* TITLE = "title";
        static constexpr const char* SIBLING_ORDER = "sibling_order";
        static constexpr const char* IMPORTANCE = "importance";
        static constexpr const char* DIFFICULTY = "difficulty";
    };
}
#endif // NOTECOLUMNS_H
