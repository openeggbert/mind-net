///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Lightweight wiki inspired by MediaWiki.
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
#ifndef CONTENTREPOSITORYIMPLSQLITE_H
#define CONTENTREPOSITORYIMPLSQLITE_H

#include "miniwiki/persistence/api/ContentRepository.h"
#include "miniwiki/models/Content.h"
#include <vector>

namespace miniwiki::impl::sqlite::repositories
{
    using std::vector;

    class ContentRepositoryImplSqlite : public persistence::api::ContentRepository
    {
    public:
        ContentRepositoryImplSqlite();
        ~ContentRepositoryImplSqlite() override;

        int create(const models::Content& content) override;
        models::Content read(int id) override;
        // void remove(int id) override;
        // void update(models::Content& content) override;
    };
}
#endif // CONTENTREPOSITORYIMPLSQLITE_H
