///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki: Lightweight wiki inspired by MediaWiki.
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
#ifndef MIGRATIONCOLUMNS_H
#define MIGRATIONCOLUMNS_H

namespace miniwiki::models::fields{
    struct MigrationColumns {
        MigrationColumns() = delete;

        MigrationColumns(const MigrationColumns &) = delete;

        MigrationColumns &operator=(const MigrationColumns &) = delete;

        static constexpr const char *MODEL_NAME = "migration";

        static constexpr const char *ID = "id";
        static constexpr const char *MAX_MIGRATION_NUMBER = "max_migration_number";
    };
}
#endif // MIGRATIONCOLUMNS_H
