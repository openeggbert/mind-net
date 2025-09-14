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

#include "mindnet/impl/sqlite/RepositoryImplSqlite.h"

#include "mindnet/api/IRepository.h"
#include "mindnet/impl/sqlite/RepositoryHelper.h"
#include "SQLiteCpp/Database.h"

namespace mindnet::impl::sqlite
{
    RepositoryImplSqlite::RepositoryImplSqlite(
        model::ModelDefinition& model_definition_
    ) : IRepository(model_definition_)
    {
    }

    RepositoryImplSqlite::~RepositoryImplSqlite() = default;

    int RepositoryImplSqlite::create(const entity_fields& fields, string& error)
    {
        try { return impl::sqlite::create_model(fields, get_model_definition(), error); }
        catch (std::exception& e) { return -1; }
    }

    entity_fields RepositoryImplSqlite::read(const int id, string& error)
    {
        return impl::sqlite::read_model(get_model_definition(), id, error);
    }

    bool RepositoryImplSqlite::update(int id, entity_fields& fields, string& error)
    {
        return impl::sqlite::update_model(id, get_model_definition(), fields, error);
    }

    bool RepositoryImplSqlite::remove(int id, string& error)
    {
        return impl::sqlite::delete_model(get_model_definition(), id, error);
    }

    std::vector<entity_fields> RepositoryImplSqlite::list(http::QueryParams& query_params, string& error)
    {
        return impl::sqlite::list_models(get_model_definition(), query_params, error);
    }

    model::ModelDefinition& RepositoryImplSqlite::get_model_definition()
    {
        return model_definition;
    }

    entity_fields RepositoryImplSqlite::request_to_entity_fields(
        crow::json::rvalue& body, plugins::core::enums::Crudl crudl)
    {
        entity_fields result;
        bool create = crudl == plugins::core::enums::Crudl::CREATE;
        bool update = crudl == plugins::core::enums::Crudl::UPDATE;
        if (!create && !update)
        {
            throw std::runtime_error(std::string("Invalid crudl ") + crudl_to_string(crudl));
        }
        //id
        result.emplace_back(0);
        //created at
        if (create) { result.emplace_back(static_cast<int64_t>(Utils::currentUnixTimestamp())); }
        else { result.emplace_back(static_cast<int64_t>(0)); }
        //updated at
        result.emplace_back(static_cast<int64_t>(Utils::currentUnixTimestamp()));


        for (auto& col : model_definition.get_columns())
        {

            auto column_name = col.get_column_name();
            if (
                column_name == model::BaseColumns::ID ||
                column_name == model::BaseColumns::CREATED_AT ||
                column_name == model::BaseColumns::UPDATED_AT
                ) continue;
            bool mandatory = col.is_mandatory();
            bool has_value = body.has(col.get_column_name());
            auto rvalue = has_value ? body[col.get_column_name()] : crow::json::rvalue();
            if (mandatory && !has_value)
            {
                throw std::runtime_error("Mandatory column " + col.get_column_name() + " is missing");
            }

            switch (find_primitive_column_type(col.get_column_type()))
            {
            case model::PrimitiveColumnType::Text:
                {
                    result.emplace_back(
                        has_value
                            ? rvalue.s()
                            : col.get_default_value());
                }
                break;
            case model::PrimitiveColumnType::Number:
                {
                    if (col.get_foreign_key().empty())
                    {
                        result.emplace_back(
                            has_value
                                ? cast64(rvalue)
                                : (col.get_default_value().empty()
                                       ? cast64(0)
                                       : cast64(std::stoi(col.get_default_value()))));
                    }
                    else
                    {
                        if (has_value && rvalue != 0)
                        {
                            result.emplace_back(cast64(rvalue));
                        }
                        else
                        {
                            result.emplace_back(FOREIGN_KEY_NULL);
                        }
                    }
                    break;
                    default: throw std::runtime_error("Unsupported type " + column_type_to_string(col.get_column_type()));
                }
            }

        }
        return result;
    }
}