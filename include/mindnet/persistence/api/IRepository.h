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
#include "mindnet/models/misc/ModelDefinition.h"
#include "mindnet/http/QueryParams.h"

#define string_for(COLUMN) body[cols::COLUMN].s()
#define int_for(COLUMN) cast64(body[cols::COLUMN])
#define mandatory_string(COLUMN) fields.emplace_back(string_for(COLUMN));
#define mandatory_int(COLUMN) fields.emplace_back(int_for(COLUMN));

#define optional_string(COLUMN, DEFAULT_VALUE) \
    if (body.has(cols::COLUMN)) {fields.emplace_back(string_for(COLUMN));}\
    else{fields.emplace_back(DEFAULT_VALUE);}

#define optional_int(COLUMN, DEFAULT_VALUE) \
if (body.has(cols::COLUMN)) {fields.emplace_back(int_for(COLUMN));}\
else{fields.emplace_back(DEFAULT_VALUE);}

#define int_foreign_key(COLUMN)\
if (\
    body.has(cols::COLUMN) &&\
    body[cols::COLUMN] != 0)\
{\
    mandatory_int(COLUMN);\
} else\
{\
    fields.emplace_back(FOREIGN_KEY_NULL);\
}

#define start_of_request_to_entity_fields(Model)\
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
fields.emplace_back(0);\
\
if (create)\
{\
    fields.emplace_back(cast64(Utils::currentUnixTimestamp()));\
}\
else\
{\
    fields.emplace_back(cast64(0));\
}\
fields.emplace_back(cast64(Utils::currentUnixTimestamp()));


namespace mindnet::persistence::api
{
    typedef entity_fields (*request_to_entity_fields_pointer)(crow::json::rvalue&, enums::Crudl);

    using std::string;

    class IRepository
    {
    public:
        virtual ~IRepository() = default;
        IRepository(
        api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer,
        models::misc::ModelDefinition& model_definition
        );
        virtual int create(const entity_fields& fields, string& error) = 0;
        virtual entity_fields read(int id, string& error) = 0;
        virtual bool update(int id, entity_fields& fields, string& error) = 0;
        virtual bool remove(int id, string& error) = 0;
        virtual std::vector<entity_fields> list(http::QueryParams& query_params, string& error) = 0;
        [[nodiscard]] virtual mindnet::models::misc::ModelDefinition& get_model_definition() = 0;
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl) =
        0;

    protected:
        request_to_entity_fields_pointer request_to_entity_fields_pointer_ = nullptr;
        models::misc::ModelDefinition model_definition;
    };
}

#endif // IREPOSITORY_H
