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
#ifndef TERMREPOSITORYIMPLSQLITE_H
#define TERMREPOSITORYIMPLSQLITE_H

#include "miniwiki/persistence/Api/TermRepository.h"
#include "miniwiki/models/Term.h"
#include <string>
#include <vector>

namespace miniwiki::Impl::Sqlite::Repositories
{
    using std::vector;

    class TermRepositoryImplSqlite : public Persistence::Api::TermRepository
    {
    public:
        TermRepositoryImplSqlite();
        ~TermRepositoryImplSqlite() override;

        void create(const Entity::Term& term) override;
        Entity::Term read(const std::string& id) override;
        void update(const Entity::Term& term) override;
        str id_for_name_and_category(str& name, str& category) override;
        void remove(const std::string& id) override;
        std::vector<Entity::Term>
        list(std::string& category, size_t pageNumber, size_t pageSize) override;
        std::vector<str> list_categories() override;
    };
}
#endif // TERMREPOSITORYIMPLSQLITE_H
