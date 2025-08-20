///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki: Lightweight wiki inspired by MediaWiki.
// Copyright (C) 2023-2023 the original author or authors.
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
#ifndef SQLITEDATABASEMIGRATION_H
#define SQLITEDATABASEMIGRATION_H

#include <string>

#include "mindnet/Utils.h"

namespace mindnet::persistence::impl::sqlite {

/**
 *
* @author <a href="mailto:mail@robertvokac.com">Robert Vokac</a>
 */
class SqliteDatabaseMigration {
private:
    //Not meant to be instantiated
    SqliteDatabaseMigration();

public:
    SqliteDatabaseMigration(const SqliteDatabaseMigration&) = delete;
    SqliteDatabaseMigration& operator=(const SqliteDatabaseMigration&) = delete;

    static SqliteDatabaseMigration* getInstance();
    static void destroyInstance();
    bool migrate();

};

}
#endif // SQLITEDATABASEMIGRATION_H
