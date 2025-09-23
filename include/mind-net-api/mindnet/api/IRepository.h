///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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
#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <nlohmann/json.hpp>

#include <string>

#include "crow/json.h"
#include "mindnet/essential/Helper.h"
#include "mindnet/model/ModelDefinition.h"
#include "mindnet/orm/QueryParams.h"

namespace mindnet::api
{
    using std::string;

    class IRepository
    {
    public:
        virtual ~IRepository() = default;
        IRepository(
            model::ModelDefinition& model_definition
        );
        virtual int create(const entity_fields& fields, string& error) = 0;
        virtual entity_fields read(int id, string& error) = 0;
        virtual bool update(int id, entity_fields& fields, string& error) = 0;
        virtual bool remove(int id, string& error) = 0;
        virtual std::vector<entity_fields> list(orm::QueryParams& query_params, string& error) = 0;
        [[nodiscard]] virtual mindnet::model::ModelDefinition& get_model_definition() = 0;
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body,
                                                       mindnet::essential::Crudl crudl) =
        0;

    protected:
        model::ModelDefinition model_definition;
    };
}

#endif // IREPOSITORY_H
