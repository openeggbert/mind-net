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
#ifndef LITERATURESOURCEREPOSITORYIMPLSQLITE_H
#define LITERATURESOURCEREPOSITORYIMPLSQLITE_H

#include "NoteBox/Persistence/Api/LiteratureSourceRepository.h"
#include "NoteBox/Persistence/Impl/SQLite/SqliteConnectionFactory.h"
#include "NoteBox/Entity/LiteratureSource.h"
#include <string>
#include <vector>

namespace NoteBox::Impl::Sqlite::Repositories
{
    using std::vector;

    class LiteratureSourceRepositoryImplSqlite : public Persistence::Api::LiteratureSourceRepository
    {
    private:
        Persistence::Impl::Sqlite::SqliteConnectionFactory* sqliteConnectionFactory;

    public:
        LiteratureSourceRepositoryImplSqlite(
            Persistence::Impl::Sqlite::SqliteConnectionFactory* sqliteConnectionFactoryIn);
        ~LiteratureSourceRepositoryImplSqlite() override;

        void create(const Entity::LiteratureSource& literature_source) override;
        vector<Entity::LiteratureSource> list() override;

        void remove(const Entity::LiteratureSource& literature_source) override;

        void update(Entity::LiteratureSource& literature_source) override;
    };
}
#endif // LITERATURESOURCEREPOSITORYIMPLSQLITE_H
