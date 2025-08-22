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
#ifndef IREPOSITORY_H
#define IREPOSITORY_H


#include <nlohmann/json.hpp>

#include <string>

#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"
#include "mindnet/models/misc/BaseModel.h"
#define def_virtual_irepository_methods \
virtual int create(const entity_fields& fields) = 0;\
virtual entity_fields read(int id) = 0;\
virtual bool update(int id, entity_fields& fields) = 0;\
virtual bool remove(int id) = 0;\
virtual std::vector<entity_fields> list(size_t page_number, size_t pageSize) = 0;\
virtual models::ModelDefinition& get_model_definition() = 0;\
virtual entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) = 0;

namespace mindnet::models {

    using std::string;

    class IRepository {

    public:
        virtual ~IRepository() = default;
        IRepository() = default;

    public:
        def_virtual_irepository_methods

    };
}

#endif // IREPOSITORY_H
