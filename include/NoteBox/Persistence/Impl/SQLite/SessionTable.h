///////////////////////////////////////////////////////////////////////////////////////////////
// bit-backup: Tool detecting bit rots in files.
// Copyright (C) 2023-2025 the original author or authors.
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


namespace NoteBox::Persistence::Impl::Sqlite {

struct LiteratureTable {


    FileTable() = delete;

    FileTable(const FileTable&) = delete;
    FileTable& operator=(const FileTable&) = delete;

    static constexpr const char* TABLE_NAME = "FILE";
    
    static constexpr const char* ID = "ID";
    static constexpr const char* NAME = "NAME";
    static constexpr const char* ABSOLUTE_PATH = "ABSOLUTE_PATH";
    static constexpr const char* LAST_MODIFICATION_DATE = "LAST_MODIFICATION_DATE";
    static constexpr const char* LAST_CHECK_DATE = "LAST_CHECK_DATE";
    //
    static constexpr const char* HASH_SUM_VALUE = "HASH_SUM_VALUE";
    static constexpr const char* HASH_SUM_ALGORITHM = "HASH_SUM_ALGORITHM";
    static constexpr const char* SIZE = "SIZE";
    static constexpr const char* LAST_CHECK_RESULT = "LAST_CHECK_RESULT";
    

};
}

