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
#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <nlohmann/json.hpp>

#include <string>

#include "mindnet/models/misc/ModelDefinition.h"
#include "mindnet/Helper.h"

#include "crow/json.h"
#include "mindnet/enums/Crudl.h"

#define PASTE(a,b) a##b
#define XPASTE(a,b) PASTE(a,b)

#define start_model_definition(Model, MODEL) \
using type = enums::ColumnType; \
using coldef = misc::ColumnDefinition; \
using def = misc::ModelDefinition; \
using bm = misc::BaseModel; \
using cols = columns::XPASTE(Model, Columns); \
using crudl = mindnet::enums::Crudl; \
\
static def XPASTE(MODEL,_DEFINITION) = def(cols::MODEL_NAME)

// Start of macros for: model definition
#define auto_inc .set_auto_inc(true)
#define start_columns_definition\
.set_columns(\
            {
#define end_columns_definition })
#define allowed_rest_operations(...) .set_operations(__VA_ARGS__)
#define end_model_definition ;
// End of macros for: model definition

#define start_model_struct(Model) struct Model : bm {\
Model () = default;

#define end_model_struct };
#define create_model_h_methods(Model, MODEL)\
[[nodiscard]] const def& get_definition() const override\
        {\
            return XPASTE(MODEL,_DEFINITION);\
        }\
\
[[nodiscard]] entity_fields get_values() const override;\
void from_values(const entity_fields& values) override;\
\
friend std::ostream& operator<<(std::ostream& os, const Model & o)\
{\
    os << o.to_json();\
    return os;\
}\

#define start_models_namespace namespace mindnet::models{
#define end_models_namespace }

#define defcol(column_name_) coldef(cols:: column_name_ )
//
#define mandatory_ .set_mandatory(true)
#define unique_ .set_unique(true)
#define foreign_key_(foreign_key_column_name) .set_foreign_key( foreign_key_column_name)
#define enum_(e) .set_enum_definition(e)
#define integer_ .integer()
#define datetime_ .datetime()
#define textarea_ .textarea()

//
#define enddefcol ,
//
#define def_helper_lambdas()\
auto number = [&values, &i]\
{\
    return std::get<std::int64_t>(values[i++]);\
};\
\
auto text = [&values, &i]\
{\
    return std::get<std::string>(values[i++]);\
};\

namespace mindnet::models::misc {

    using std::string;


    struct BaseModel {

    protected:
        int id{};
        unixtime created_at{};
        unixtime updated_at{};

    public:

        virtual ~BaseModel() = default;
        [[nodiscard]] int get_id() const
        {
            return id;
        }

        void set_id(const int id_)
        {
            id = id_;
        }
        [[nodiscard]] virtual const misc::ModelDefinition& get_definition() const = 0;

        [[nodiscard]] virtual entity_fields get_values() const = 0;
        virtual void from_values(const entity_fields& values) = 0;

        [[nodiscard]] JSON to_json() const
        {
            JSON json;
            int index = 0;
            entity_fields fields = get_values();
            auto definition = get_definition();

            for (auto& e : definition.get_columns())
            {
                auto field = e.get_column_name();
                std::visit([&json, &field](const auto& value)
                {
                    json[field] = value;
                }, fields[index]);
                index++;
            }
            return json;
        };

        // [[nodiscard]] bool equals(const BaseModel &other) const
        // {
        //     return to_json() == other.to_json();
        // };
        void print(std::ostream &os) const
        {
            os << to_json();
        };
    };
}

#endif // BASEMODEL_H
