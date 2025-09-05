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



//
#define test_true(condition, error) if ( ! ( condition ) ) return error;
//
#define test_eq(number, eq_to, field) test_true(number == eq_to, field " must be equal to " + std::to_string(eq_to))
#define test_ne(number, eq_to, field) test_true(number != eq_to, field " must be not equal to " + std::to_string(eq_to))
#define test_at_least(number, min_value, field) test_true(number >= min_value, field " must not be less than " + std::to_string(min_value))
#define test_at_most(number, max_value, field) test_true(number <= max_value, field " must not be greater than " + std::to_string(max_value))
#define test_between(number, min_value, max_value, field) test_at_least(number, min_value, field) test_at_most(number, max_value, field)
//
#define testt_at_least(text, min_value, field) test_at_least(text .size(), min_value, field)
#define testt_at_most(text, max_value, field) test_at_most(text .size(), max_value, field)
#define testt_between(text, min_value, max_value, field) test_between(text .size(), min_value, max_value, field)
//
#define testt_not_empty(text, field) test_true(!text.empty(), field " must not be empty")
//


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
}

#endif // BASEMODEL_H