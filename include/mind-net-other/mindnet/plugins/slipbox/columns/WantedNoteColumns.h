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
#ifndef WANTEDNOTECOLUMNS_H
#define WANTEDNOTECOLUMNS_H

#include "../../../model/BaseColumns.h"
#include "mindnet/essential/Helper.h"
#include "../../../model/ColumnType.h"

namespace mindnet::plugins::slipbox::columns
{
    struct WantedNoteColumns : model::BaseColumns
    {
        WantedNoteColumns() = delete;

        WantedNoteColumns(const WantedNoteColumns&) = delete;
        WantedNoteColumns& operator=(const WantedNoteColumns&) = delete;

        static constexpr const char* MODEL_NAME = "wanted_note";

        static constexpr const char* FROM_NOTE_ID = "from_note_id";
        static constexpr const char* TO_NOTE_TITLE = "to_note_title";
    };
}
#endif // WANTEDNOTECOLUMNS_H
