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
#ifndef OLDENTITYTABLE_H
#define OLDENTITYTABLE_H


namespace NoteBox::Persistence::Impl::Sqlite::Tables
{
    struct OldEntityTable
    {
        OldEntityTable() = delete;

        OldEntityTable(const OldEntityTable&) = delete;
        OldEntityTable& operator=(const OldEntityTable&) = delete;

        static constexpr const char* TABLE_NAME = "OLD_ENTITY";

        static constexpr const char* ID = "ID";
        static constexpr const char* ENTITY_NAME = "ENTITY_NAME";
        static constexpr const char* ENTITY_ID = "ENTITY_ID";
        static constexpr const char* VALUE = "VALUE";
        static constexpr const char* ADDED_AT = "ADDED_AT";
    };
}

#endif // OLDENTITYTABLE_H
