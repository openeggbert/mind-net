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
#ifndef REPOSITORYIMPLSQLITE_H
#define REPOSITORYIMPLSQLITE_H

#include <vector>

#include "../../../plugins/core/enums/Crudl.h"
#include "mindnet/persistence/api/Persistence.h"
#include "mindnet/persistence/api/IRepository.h"

namespace mindnet::persistence::impl::sqlite
{
    using std::vector;

    class RepositoryImplSqlite : public api::IRepository
    {
    public:
        RepositoryImplSqlite(
            api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer,
            model::ModelDefinition& model_definition
            );
        ~RepositoryImplSqlite() override;

        int create(const entity_fields& fields, string& error) override;
        entity_fields read(int id, string& error) override;
        bool update(int id, entity_fields& fields, string& error) override;
        bool remove(int id, string& error) override;
        std::vector<entity_fields> list(http::QueryParams& query_params, string& error) override;
        [[nodiscard]] model::ModelDefinition& get_model_definition() override;
        entity_fields request_to_entity_fields(crow::json::rvalue& body, plugins::core::enums::Crudl crudl) override;
    };
}
#endif // REPOSITORYIMPLSQLITE_H
