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

#include "mindnet/util/TestUtils.hpp"

#define create_model_h_methods(Model, MODEL)\
int get_field_count()\
        {\
            return std::tuple_size_v<decltype(fields)>;\
        }\
\
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

#define create_model_cpp_methods(Model) \
entity_fields Model::to_values() const {return serialize_fields(*this);} \
void Model::from_values(const entity_fields& values){deserialize_fields(*this, values);} \

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

    // ---------- GENERIC SERIALIZATION HELPERS ----------
    template<typename FieldType>
    auto serialize_value(const FieldType& value);
    template<typename FieldType>
    FieldType deserialize_value(const entity_field& input);

    template<typename Obj, typename MemberPtr>
using member_type_t =
    std::decay_t<decltype(std::declval<Obj>().*std::declval<MemberPtr>())>;

    struct BaseModel
    {
    protected:
        identification id{};
        unixtime created_at{};
        unixtime updated_at{};

        static constexpr auto base_fields = std::make_tuple(
            &BaseModel::id,
            &BaseModel::created_at,
            &BaseModel::updated_at
        );
        static constexpr auto base_fields_size = std::tuple_size_v<decltype(base_fields)>;

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
        }

        bool operator<(const BaseModel& other) const
        {
            return id < other.id;
        }

        int get_column_index(const char* column_name) const
        {
            auto& def = get_model_definition();
            return def.get_column_index(column_name);
        }

        bool operator==(const BaseModel& other) const
        {
            return id == other.id
                && created_at == other.created_at
                && updated_at == other.updated_at;
        }

    protected:
        template<typename T>
        entity_fields serialize_fields(const T& obj) const
        {
            // Combine base and user-defined fields into a single tuple
            constexpr auto combined_fields = std::tuple_cat(BaseModel::base_fields, T::fields);

            static constexpr auto total_size = std::tuple_size_v<decltype(combined_fields)>;

            entity_fields out;
            out.reserve(total_size);

            // One single std::apply over all fields
            std::apply([&](auto... memptr){
                (
                    out.push_back( serialize_value(obj.*memptr) ),
                ...);
            }, combined_fields);

            return out;
        }

        template<typename T>
        void deserialize_fields(T& obj, const entity_fields& values)
        {
            constexpr auto combined_fields = std::tuple_cat(BaseModel::base_fields, T::fields);
            static constexpr auto total_size = std::tuple_size_v<decltype(combined_fields)>;

            if (values.size() != total_size)
                throw std::runtime_error("Invalid field count for model " + get_model_definition().get_model_name());

            int i = 0;

            std::apply([&](auto... memptr){
                (
                    [&]{
                        using F = member_type_t<T, decltype(memptr)>;
                        obj.*memptr = deserialize_value<F>(values[i++]);
                    }(),
                ...);
            }, combined_fields);
        }

    };

    inline string validate_enums(const entity_fields& fields_, const ModelDefinition& def_)
    {
        const auto& columns = def_.get_columns();
        for (int i = 0; i < fields_.size(); i++)
        {
            const auto& column = columns[i];

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

        const auto& columns = def_.get_columns();
        for (int i = 0; i < old_.size(); i++)
        {
            const auto& column = columns[i];
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

        const auto& columns = def_.get_columns();
        for (int i = 0; i < new_.size(); i++)
        {
            const auto& column = columns[i];
            if (!column.is_internal()) continue;
            if (old_[i] != new_[i])
                return "Value of column " + column.get_column_name() + " is internal and cannot be changed by user.";
        }
        return "";
    }

    inline string validate_internal(entity_fields& new_, ModelDefinition& def_)
    {
        const auto& columns = def_.get_columns();
        for (int i = 0; i < new_.size(); i++)
        {
            const auto& column = columns[i];
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

    // ---------- GENERIC SERIALIZATION HELPERS ----------
    template<typename FieldType>
    auto serialize_value(const FieldType& value)
    {
        if constexpr (std::is_enum_v<FieldType>) {
            return (int64_t) static_cast<std::underlying_type_t<FieldType>>(value);
        }
        else if constexpr (std::is_same_v<FieldType, bool>) {
            return value;
        }
        else if constexpr (std::is_convertible_v<FieldType, std::string>) {
            return std::string(value);
        }
        else if constexpr (std::is_arithmetic_v<FieldType>) {
            return (int64_t) value;
        }
        else {
            static_assert(!sizeof(FieldType), "Unsupported field type in serialize_value");
        }
    }

    template<typename FieldType>
    FieldType deserialize_value(const entity_field& input)
    {
        if constexpr (std::is_enum_v<FieldType>) {
            using UT = std::underlying_type_t<FieldType>;
            return static_cast<FieldType>((UT) std::get<int64_t>(input));
        }
        else if constexpr (std::is_same_v<FieldType, bool>) {
            return std::get<int64_t>(input) != 0;
        }
        else if constexpr (std::is_convertible_v<std::string, FieldType>) {
            return std::get<std::string>(input);
        }
        else if constexpr (std::is_arithmetic_v<FieldType>) {
            return (FieldType) std::get<int64_t>(input);
        }
        else {
            static_assert(!sizeof(FieldType), "Unsupported type in deserialize_value");
        }
        throw std::runtime_error("Illegal state");
    }

}