/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <nlohmann/json.hpp>

#include <string>

#include "ColumnDefinition.hpp"
#include "ColumnType.hpp"
#include "ModelDefinition.hpp"
#include "mindnet/essential/DatabaseType.hpp"

#include "mindnet/util/TestUtils.hpp"

#define create_model_h_methods(Model, MODEL)\
[[nodiscard]] const def& get_model_definition() const override\
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
auto boolean = [&number]\
{\
return number() != 0;\
};\
\
auto text = [&values, &i]\
{\
    return std::get<std::string>(values[i++]);\
};

#define using_test_utils()\
using util::ValidatorChain;
using mindnet::util::validator_chain_vector;\
using mindnet::util::test_ok;\
using mindnet::util::test_ko;\
using mindnet::util::test_true;\
using mindnet::util::test_false;\
using mindnet::util::test_eq;\
using mindnet::util::test_ne;\
using mindnet::util::test_not_null;\
using mindnet::util::test_at_least;\
using mindnet::util::test_at_most;\
using mindnet::util::test_between;\
using mindnet::util::testt_at_least;\
using mindnet::util::testt_at_most;\
using mindnet::util::testt_between;\
using mindnet::util::testt_not_empty;\
using mindnet::util::testt_is_alpha_or_digit;

namespace mindnet::model
{
    typedef nlohmann::json JSON;

    [[nodiscard]] JSON model_to_json(
        entity_fields fields,
        const ModelDefinition& definition);

    using std::string;
    using type = ColumnType;
    using coldef = ColumnDefinition;
    using def = ModelDefinition;
    using crudl = mindnet::essential::Crudl;

    struct BaseModel
    {
    protected:
        identification id{};
        unixtime created_at{};
        unixtime updated_at{};

    public:
        virtual ~BaseModel() = default;

        [[nodiscard]] identification get_id() const
        {
            return id;
        }

        void set_id(const identification id_)
        {
            id = id_;
        }

        [[nodiscard]] unixtime get_created_at() const
        {
            return created_at;
        }

        [[nodiscard]] unixtime get_updated_at() const
        {
            return updated_at;
        }

        [[nodiscard]] virtual const ModelDefinition& get_model_definition() const = 0;

        [[nodiscard]] virtual entity_fields to_values() const = 0;
        virtual void from_values(const entity_fields& values) = 0;
        virtual string validate() = 0;

        [[nodiscard]] JSON to_json() const
        {
            return model_to_json(to_values(), get_model_definition());
        };

        // [[nodiscard]] bool equals(const BaseModel &other) const
        // {
        //     return to_json() == other.to_json();
        // };
        void print(std::ostream& os) const
        {
            os << to_json();
        };

        bool operator<(const BaseModel& other) const
        {
            return id < other.id;
        }

        int get_column_index(const char* column_name) const
        {
            auto& def = get_model_definition();
            return def.get_column_index(column_name);
        }
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
                    return "Invalid enum value " + std::to_string(value_int64_t) + " for column '" + column.
                        get_column_name() +
                        "'. Please provide a valid value from the allowed enum options.";
                }
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
            if (column.get_column_name() == BaseColumns::CREATED_AT) continue;
            if (column.is_readonly() && old_[i] != new_[i])
            {
                return "Value of column " + column.get_column_name() + " is readonly and cannot be changed.";
            }
        }
        return "";
    }

    inline string validate_internal(entity_fields& old_, entity_fields& new_, ModelDefinition& def_)
    {
        if (old_.size() != new_.size()) return "The number of fields in the entity has changed";

        auto columns = def_.get_columns();
        for (int i = 0; i < new_.size(); i++)
        {
            auto column = columns[i];
            if (!column.is_internal()) continue;
            if (old_[i] != new_[i])
                return "Value of column " + column.get_column_name() + " is internal and cannot be changed by user.";
        }
        return "";
    }

    inline string validate_internal(entity_fields& new_, ModelDefinition& def_)
    {
        auto columns = def_.get_columns();
        for (int i = 0; i < new_.size(); i++)
        {
            auto column = columns[i];
            if (!column.is_internal()) continue;
            auto value = new_[i];
            auto value_int64_t = std::get_if<int64_t>(&value);
            auto value_string = std::get_if<string>(&value);
            if (value_int64_t && *value_int64_t != 0)
            {
                return "Value of column " + column.get_column_name() + " is internal and cannot be set by user.";
            }
            if (value_string && *value_string != "")
            {
                return "Value of column " + column.get_column_name() + " is internal and cannot be set by user.";
            }
            if (!value_int64_t && !value_string)
            {
                return "Value of column " + column.get_column_name() + " has unsupported std::variant type.";
            }
        }
        return "";
    }
}