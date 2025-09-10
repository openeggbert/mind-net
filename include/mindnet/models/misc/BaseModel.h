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
#include "mindnet/TestUtils.h"

#define create_model_h_methods(Model, MODEL)\
[[nodiscard]] const def& get_definition() const override\
        {\
            return XPASTE(MODEL,_DEFINITION);\
        }\
\
[[nodiscard]] entity_fields to_values() const override;\
void from_values(const entity_fields& values) override;\
string validate() override;\
\
friend std::ostream& operator<<(std::ostream& os, const Model & o)\
{\
    os << o.to_json();\
    return os;\
}\
\
Model() = default;

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
};

namespace mindnet::models::misc
{
    using std::string;
    using type = enums::ColumnType;
    using coldef = misc::ColumnDefinition;
    using def = misc::ModelDefinition;
    using crudl = mindnet::enums::Crudl;

    struct BaseModel
    {
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

        [[nodiscard]] virtual entity_fields to_values() const = 0;
        virtual void from_values(const entity_fields& values) = 0;
        virtual string validate() = 0;

        [[nodiscard]] JSON to_json() const
        {
            JSON json;
            int index = 0;
            entity_fields fields = to_values();
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
        void print(std::ostream& os) const
        {
            os << to_json();
        };

    };

    inline string validate_enums(const entity_fields& fields_, const ModelDefinition& def_)
    {
        auto columns = def_.get_columns();
        for (int i = 0; i < fields_.size(); i++)
        {
            auto column = columns[i];

            if (column.get_enum_definition().has_value())
            {
                auto value = fields_[i];
                int64_t value_int64_t = std::get<int64_t>(value);

                if (!column.get_enum_definition()->is_value_valid(value_int64_t))
                {
                }
                return "Invalid enum value for column '" + column.get_column_name() +
                    "'. Please provide a valid value from the allowed enum options.";
            }
        }
        return "";
    }
    
    inline string validate_readonly(entity_fields& old_, entity_fields& new_, ModelDefinition& def_)
    {
        if (old_.size() != new_.size()) return "The number of fields in the entity has changed";
        auto columns = def_.get_columns();
        for (int i = 0; i < old_.size(); i++)
        {
            auto column = columns[i];
            if (column.is_readonly() && old_[i] != new_[i])
            {
                return "Value of column " + column.get_column_name() + " is readonly and cannot be changed.";
            }
        }
        return "";
    }
}

#endif // BASEMODEL_H