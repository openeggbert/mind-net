///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Note management tool.
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
#ifndef LITERATURESOURCETABLE_H
#define LITERATURESOURCETABLE_H


namespace miniwiki::Persistence::Impl::Sqlite::Tables {

struct LiteratureSourceTable {


    LiteratureSourceTable() = delete;

    LiteratureSourceTable(const LiteratureSourceTable&) = delete;
    LiteratureSourceTable& operator=(const LiteratureSourceTable&) = delete;

    static constexpr const char* TABLE_NAME = "LITERATURE_SOURCE";
    
    static constexpr const char* ID = "ID";
    static constexpr const char* TITLE = "TITLE";
    static constexpr const char* AUTHOR = "AUTHOR";
    static constexpr const char* YEAR = "YEAR";
    static constexpr const char* PUBLICATION = "PUBLICATION";
    //
    static constexpr const char* URL = "URL";
    static constexpr const char* CONTENT = "CONTENT";
    

};
}

#endif // LITERATURESOURCETABLE_H
