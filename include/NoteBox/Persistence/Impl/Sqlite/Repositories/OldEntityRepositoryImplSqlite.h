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
#ifndef OLDENTITYREPOSITORYIMPLSQLITE_H
#define OLDENTITYREPOSITORYIMPLSQLITE_H


#include "NoteBox/Persistence/Api/OldContentRepository.h"
#include <string>
#include <vector>

#include "NoteBox/Entity/OldEntity.h"

namespace NoteBox::Impl::Sqlite::Repositories
{
    using std::vector;

    class OldEntityRepositoryImplSqlite : public Persistence::Api::OldEntityRepository
    {
    public:
        OldEntityRepositoryImplSqlite();
        ~OldEntityRepositoryImplSqlite() override;

         void create(const Entity::OldEntity& content) override;
         Entity::OldEntity read(std::string& entity_name, unixtime added_at) override;

         void remove(std::string& entity_name, unixtime added_at) override;
         std::vector<unixtime> list_timestamps(std::string& entity_name, std::string& entity_id ) override;
    };
}
#endif // OLDENTITYREPOSITORYIMPLSQLITE_H
