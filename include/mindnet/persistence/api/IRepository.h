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
#define def_virtual_irepository_api_h_methods \
virtual int create(const entity_fields& fields) = 0;\
virtual entity_fields read(int id) = 0;\
virtual bool update(int id, entity_fields& fields) = 0;\
virtual bool remove(int id) = 0;\
virtual std::vector<entity_fields> list(size_t page_number, size_t pageSize) = 0;\
virtual models::ModelDefinition& get_model_definition() = 0;\
virtual entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) = 0;

#define def_virtual_irepository_impl_h_methods \
int create(const entity_fields& fields) override;\
entity_fields read(int id) override;\
bool update(int id, entity_fields& fields) override;\
bool remove(int id) override;\
std::vector<entity_fields> list(size_t page_number, size_t pageSize) override;\
models::ModelDefinition& get_model_definition() override;\
entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) override;

#define def_virtual_irepository_impl_cpp_methods(Model, MODEL) \
int Model##RepositoryImplSqlite::create(const entity_fields& fields)\
    {\
        try\
        {\
            return persistence::impl::sqlite::create_model(fields, get_model_definition());\
        }\
        catch (std::exception& e)\
        {\
            return -1;\
        }\
    }\
\
entity_fields Model##RepositoryImplSqlite::read(const int id)\
    {\
        return persistence::impl::sqlite::read_model(get_model_definition(), id);\
\
    }\
   \
bool Model##RepositoryImplSqlite::update(int id, entity_fields& fields)\
    {\
        return persistence::impl::sqlite::update_model(id, get_model_definition(), fields);\
    }\
\
bool Model##RepositoryImplSqlite::remove(int id)\
    {\
        return persistence::impl::sqlite::delete_model(get_model_definition(), id);\
    }\
\
std::vector<entity_fields> Model##RepositoryImplSqlite::list(size_t page_number, size_t pageSize)\
    {\
        return persistence::impl::sqlite::list_models(get_model_definition(), page_number, pageSize);\
    }\
models::ModelDefinition& Model##RepositoryImplSqlite::get_model_definition()\
    {\
        return models::MODEL##_DEFINITION;\
    }\

#define string_for(COLUMN) body[cols::COLUMN].s()
#define int_for(COLUMN) cast64(body[cols::COLUMN])
#define add_string(COLUMN) fields.push_back(string_for(COLUMN));
#define add_int(COLUMN) fields.push_back(int_for(COLUMN));



#define start_of_convert_crow_json_rvalue_to_entity_fields(Model)\
typedef models::columns::Model##Columns cols;\
        entity_fields fields;\
\
bool create = crudl == enums::Crudl::CREATE;\
bool update = crudl == enums::Crudl::UPDATE;\
if (!create && !update)\
{\
    return fields;\
}\
\
fields.push_back(0);\
\
if (create)\
{\
    fields.push_back(cast64(Utils::currentUnixTimestamp()));\
}\
else\
{\
    fields.push_back(body[cols::CREATED_AT].s());\
}\
fields.push_back(cast64(Utils::currentUnixTimestamp()));


namespace mindnet::models {

    using std::string;

    class IRepository {

    public:
        virtual ~IRepository() = default;
        IRepository() = default;

    public:
        def_virtual_irepository_api_h_methods

    };
}

#endif // IREPOSITORY_H
