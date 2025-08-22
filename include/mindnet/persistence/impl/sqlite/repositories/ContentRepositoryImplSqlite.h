///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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

#include "mindnet/persistence/api/ContentRepository.h"
#include "mindnet/models/Content.h"
#include <vector>

#include "mindnet/enums/Crudl.h"

namespace mindnet::impl::sqlite::repositories
{
    using std::vector;

    class ContentRepositoryImplSqlite : public persistence::api::ContentRepository
    {
    public:
        ContentRepositoryImplSqlite();
        ~ContentRepositoryImplSqlite() override;

        int create(const entity_fields& fields) override;
        entity_fields read(int id) override;
        bool update(int id, entity_fields& fields) override;
        bool remove(int id) override;
        std::vector<entity_fields> list(size_t page_number, size_t pageSize) override;

        models::ModelDefinition& get_model_definition() override;
        entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) override;


        // void update(models::Content& content) override;
    };
}
#endif // CONTENTREPOSITORYIMPLSQLITE_H
