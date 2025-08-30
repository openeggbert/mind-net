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

#include "mindnet/persistence/impl/sqlite/repositories/RepositoryImplSqlite.h"

#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "SQLiteCpp/Database.h"

namespace mindnet::persistence::impl::sqlite::repositories
{
    RepositoryImplSqlite::RepositoryImplSqlite(
        models::convert_rest_request_to_entity_fields convert_rest_request_to_entity_fields_pointer_,
        persistence::ModelDefinition& model_definition_
    )
    {
        if (convert_rest_request_to_entity_fields_pointer == nullptr)
        {
            throw std::invalid_argument("convert_rest_request_to_entity_fields_pointer cannot be null");
        }
        convert_rest_request_to_entity_fields_pointer = convert_rest_request_to_entity_fields_pointer_,
        model_definition = model_definition_;
    };
    RepositoryImplSqlite::~RepositoryImplSqlite() = default;

    int RepositoryImplSqlite::create(const entity_fields& fields, string& error)
    {
        try { return persistence::impl::sqlite::create_model(fields, get_model_definition(), error); }
        catch (std::exception& e) { return -1; }
    }

    entity_fields RepositoryImplSqlite::read(const int id, string& error)
    {
        return persistence::impl::sqlite::read_model(get_model_definition(), id, error);
    }

    bool RepositoryImplSqlite::update(int id, entity_fields& fields, string& error)
    {
        return persistence::impl::sqlite::update_model(id, get_model_definition(), fields, error);
    }

    bool RepositoryImplSqlite::remove(int id, string& error)
    {
        return persistence::impl::sqlite::delete_model(get_model_definition(), id, error);
    }

    std::vector<entity_fields> RepositoryImplSqlite::list(http::QueryParams& query_params, string& error)
    {
        return persistence::impl::sqlite::list_models(get_model_definition(), query_params, error);
    }

    models::misc::ModelDefinition& RepositoryImplSqlite::get_model_definition()
    {
        return model_definition;
    }

    entity_fields RepositoryImplSqlite::convert_crow_json_rvalue_to_entity_fields(
        crow::json::rvalue& body, enums::Crudl crudl)
    {
        return convert_rest_request_to_entity_fields_pointer(body, crudl);
    }
}
