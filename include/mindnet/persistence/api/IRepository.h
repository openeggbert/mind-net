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

#include "crow/json.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"
#include "mindnet/models/misc/BaseModel.h"
#include "mindnet/models/misc/ModelDefinition.h"
#include "mindnet/http/QueryParams.h"

#define def_virtual_irepository_api_h_methods \
virtual int create(const entity_fields& fields, str& error) = 0;\
virtual entity_fields read(int id, str& error) = 0;\
virtual bool update(int id, entity_fields& fields, str& error) = 0;\
virtual bool remove(int id, str& error) = 0;\
virtual std::vector<entity_fields> list(http::QueryParams& query_params, str& error) = 0;\
virtual mindnet::models::misc::ModelDefinition& get_model_definition() = 0;\
virtual entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) = 0;

#define def_virtual_irepository_impl_h_methods \
int create(const entity_fields& fields, str& error) override;\
entity_fields read(int id, str& error) override;\
bool update(int id, entity_fields& fields, str& error) override;\
bool remove(int id, str& error) override;\
std::vector<entity_fields> list(http::QueryParams& query_params, str& error) override;\
models::misc::ModelDefinition& get_model_definition() override;\
entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) override;

#define def_virtual_irepository_impl_cpp_methods(Model, MODEL) \
int Model##RepositoryImplSqlite::create(const entity_fields& fields, str& error)\
    {\
        try\
        {\
            return persistence::impl::sqlite::create_model(fields, get_model_definition(), error);\
        }\
        catch (std::exception& e)\
        {\
            return -1;\
        }\
    }\
\
entity_fields Model##RepositoryImplSqlite::read(const int id, str& error)\
    {\
        return persistence::impl::sqlite::read_model(get_model_definition(), id, error);\
\
    }\
   \
bool Model##RepositoryImplSqlite::update(int id, entity_fields& fields, str& error)\
    {\
        return persistence::impl::sqlite::update_model(id, get_model_definition(), fields, error);\
    }\
\
bool Model##RepositoryImplSqlite::remove(int id, str& error)\
    {\
        return persistence::impl::sqlite::delete_model(get_model_definition(), id, error);\
    }\
\
std::vector<entity_fields> Model##RepositoryImplSqlite::list(http::QueryParams& query_params, str& error)\
    {\
        return persistence::impl::sqlite::list_models(get_model_definition(), query_params, error);\
    }\
models::misc::ModelDefinition& Model##RepositoryImplSqlite::get_model_definition()\
    {\
        return models::MODEL##_DEFINITION;\
    }
#define string_for(COLUMN) body[cols::COLUMN].s()
#define int_for(COLUMN) cast64(body[cols::COLUMN])
#define add_string(COLUMN) fields.push_back(string_for(COLUMN));
#define add_int(COLUMN) fields.push_back(int_for(COLUMN));

#define add_optional_string(COLUMN, DEFAULT_VALUE) \
    if (body.has(cols::COLUMN)) {fields.push_back(string_for(COLUMN));}\
    else{fields.push_back(DEFAULT_VALUE);}

#define add_optional_int(COLUMN, DEFAULT_VALUE) \
if (body.has(cols::COLUMN)) {fields.push_back(int_for(COLUMN));}\
else{fields.push_back(DEFAULT_VALUE);}

#define add_int_foreign_key(COLUMN)\
if (\
    body.has(cols::COLUMN) &&\
    body[cols::COLUMN] != 0)\
{\
    add_int(COLUMN);\
} else\
{\
    fields.push_back(FOREIGN_KEY_NULL);\
}


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
    fields.push_back(cast64(0));\
}\
fields.push_back(cast64(Utils::currentUnixTimestamp()));


namespace mindnet::models
{
    using std::string;

    class IRepository
    {
    public:
        virtual ~IRepository() = default;
        IRepository() = default;

    public:
        def_virtual_irepository_api_h_methods
    };
}

#endif // IREPOSITORY_H
