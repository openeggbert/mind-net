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

#include <cstring>
#include <optional>
#include <string>
#include <vector>
#include <set>

#include "BaseColumns.hpp"
#include "ColumnType.hpp"
#include "CustomAction.hpp"
#include "EnumDefinition.hpp"

#define using_flags()\
using hive::model::MANDATORY;\
using hive::model::UNIQUE;\
using hive::model::FOREIGN_KEY;\
using hive::model::AUTO;\
using hive::model::HIDDEN;\
using hive::model::READONLY;\
using hive::model::MUTABLE;\
using hive::model::INTERNAL;\
using hive::model::TEXT;\
using hive::model::TEXTAREA;\
using hive::model::INTEGER;\
using hive::model::REAL;\
using hive::model::BLOB;\
using hive::model::BOOL;\
using hive::model::DATETIME;

namespace hive::model
{
    enum ColumnDefinitionFlag
    {
        MANDATORY = 1 << 0,
        UNIQUE = 1 << 1,
        FOREIGN_KEY = 1 << 2,
        AUTO = 1 << 3,
        HIDDEN = 1 << 4,
        READONLY = 1 << 5,
        MUTABLE = 1 << 6,
        INTERNAL = 1 << 7,
        TEXT = 1 << 8,
        TEXTAREA = 1 << 9,
        INTEGER = 1 << 10,
        REAL = 1 << 11,
        BLOB = 1 << 12,
        BOOL = 1 << 13,
        DATETIME = 1 << 14
    };

    inline std::vector<ColumnDefinitionFlag> column_definition_flag_values()
    {
        return {
            MANDATORY,
            UNIQUE,
            FOREIGN_KEY,
            AUTO,
            HIDDEN,
            READONLY,
            MUTABLE,
            INTERNAL,
            TEXT,
            TEXTAREA,
            INTEGER,
            REAL,
            BLOB,
            BOOL,
            DATETIME,
        };
    }

    struct ColumnDefinition
    {
    private:
        std::string column_name;
        ColumnType column_type = ColumnType::Text;
        bool mandatory = false;
        bool primary_key = false;
        std::string foreign_key;
        std::optional<EnumDefinition> enum_definition;
        bool unique = false;
        bool auto_ = false;
        bool hidden = false;
        bool readonly = false;
        bool mutable_ = false;
        bool internal_ = false;
        std::string default_value;
        string description;

        bool ends_with_id(const char* str)
        {
            if (!str) return false;

            size_t len = std::strlen(str);
            if (len < 3) return false;

            return std::strcmp(str + (len - 3), "_id") == 0;
        }

        bool ends_with_id(const std::string& s)
        {
            return s.size() >= 3 && s.compare(s.size() - 3, 3, "_id") == 0;
        }

    public:
        /**
         * Constructs a column definition with name and type
         * @param name The column name
         * @param type The column type (defaults to TEXT)
         *
         * Special handling for certain column names:
         * - id, created_at, updated_at or columns ending with _id are set to INTEGER type
         * - id column is set as primary key and mandatory
         * - created_at column is set as mandatory
         */
        ColumnDefinition(const char* name, int flags_ = 0)
        {
            column_name = name;
            using bc = BaseColumns;
            if (ends_with_id(column_name))
            {
                column_type = hive::model::ColumnType::Integer;
            }

            if (column_name == bc::ID)
            {
                primary_key = true;
                mandatory = true;
                unique = true;
                auto_ = true;
                column_type = hive::model::ColumnType::Integer;
                description = "Unique identifier of the record.";
                readonly = true;
            }
            if (column_name == bc::CREATED_AT)
            {
                mandatory = true;
                auto_ = true;
                column_type = hive::model::ColumnType::DateTime;
                description = "Timestamp when the record was created.";
                readonly = true;
            }
            if (column_name == bc::UPDATED_AT)
            {
                auto_ = true;
                column_type = hive::model::ColumnType::DateTime;
                description = "Timestamp of the last update to the record.";
            }
            flags(flags_);
        }

        // Setters
        /** @return The column name */
        [[nodiscard]] const std::string& get_column_name() const
        {
            return column_name;
        }

        /** @return The column type */
        [[nodiscard]] const hive::model::ColumnType get_column_type() const
        {
            return column_type;
        }

        /** @return Whether the column is mandatory */
        [[nodiscard]] const bool is_mandatory() const
        {
            return mandatory;
        }

        /** @return Whether the column is a primary key */
        [[nodiscard]] const bool is_primary_key() const
        {
            return primary_key;
        }

        /** @return The column foreign_key */
        [[nodiscard]] const std::string get_foreign_key() const
        {
            return foreign_key;
        }

        /** @return The column foreign_key */
        [[nodiscard]] const bool is_foreign_key() const
        {
            return !foreign_key.empty();
        }

        /** @return The enum definition */
        [[nodiscard]] const std::optional<EnumDefinition>& get_enum_definition() const
        {
            return enum_definition;
        }

        [[nodiscard]] const bool is_unique() const
        {
            return unique || primary_key;
        }

        [[nodiscard]] const std::string get_default_value() const
        {
            return default_value;
        }

        [[nodiscard]] const i64 get_default_i64_value() const
        {
            return default_value.empty() ? 0 : std::stoll(default_value);
        }

        [[nodiscard]] const bool is_auto() const
        {
            return auto_;
        }

        [[nodiscard]] const bool is_hidden() const
        {
            return hidden;
        }

        [[nodiscard]] const bool is_readonly() const
        {
            return readonly;
        }

        [[nodiscard]] const bool is_mutable() const
        {
            return mutable_;
        }

        [[nodiscard]] const bool is_internal() const
        {
            return internal_;
        }

        [[nodiscard]] const string& get_description() const
        {
            return description;
        }

        // Setters

        /**
     * Sets the enum definition
     * @param value New enum definition
     * @return Reference to this object for method chaining
     */
        ColumnDefinition& set_enum_definition(std::optional<EnumDefinition> value)
        {
            enum_definition = value;
            column_type = hive::model::ColumnType::Integer;
            return *this;
        }

        /**
     * Sets the foreign key
     * @param value foreign key
     * @return Reference to this object for method chaining
     */
        ColumnDefinition& set_foreign_key(std::string value)
        {
            foreign_key = value;
            column_type = hive::model::ColumnType::Integer;
            return *this;
        }

        ColumnDefinition& set_default_value(std::string value)
        {
            default_value = value;
            return *this;
        }

        ColumnDefinition& set_default_value(int value)
        {
            default_value = std::to_string(value);
            return *this;
        }

        ColumnDefinition& set_default_value(bool value)
        {
            return set_default_value(value ? 1 : 0);
        }

        ColumnDefinition& set_description(string str)
        {
            description = str;
            return *this;
        }

    private:
        ColumnDefinition& set_foreign_key()
        {
            if (column_name.ends_with("_id"))
            {
                foreign_key = column_name.substr(0, column_name.size() - 3);
                column_type = hive::model::ColumnType::Integer;
            }
            else
            {
                throw std::invalid_argument(
                    "foreign_key must be set to the name of the column ending with _id, if set_foreign_key(string) method is called");
            }
            return *this;
        }

        ColumnDefinition& set_auto()
        {
            auto_ = true;
            return *this;
        }

        ColumnDefinition& set_hidden()
        {
            hidden = true;
            return *this;
        }

        ColumnDefinition& set_readonly()
        {
            readonly = true;
            return *this;
        }

        ColumnDefinition& set_mutable()
        {
            mutable_ = true;
            return *this;
        }

        ColumnDefinition& set_internal()
        {
            internal_ = true;
            return *this;
        }

        ColumnDefinition& flags(int flags)
        {
            if (flags == 0)
            {
                return *this;
            }
            std::set<ColumnDefinitionFlag> flags_set;
            for (auto cdf : column_definition_flag_values())
            {
                if (flags & cdf)
                {
                    flags_set.insert(cdf);
                }
            }
            if (flags_set.contains(MANDATORY)) mandatory = true;
            if (flags_set.contains(UNIQUE)) unique = true;
            if (flags_set.contains(FOREIGN_KEY)) set_foreign_key();
            if (flags_set.contains(AUTO)) set_auto();
            if (flags_set.contains(HIDDEN)) set_hidden();
            if (flags_set.contains(READONLY)) set_readonly();
            if (flags_set.contains(MUTABLE)) set_mutable();
            if (flags_set.contains(INTERNAL)) set_internal();
            //
            if (flags_set.contains(TEXT)) column_type = hive::model::ColumnType::Text;
            if (flags_set.contains(TEXTAREA)) column_type = hive::model::ColumnType::TextArea;
            if (flags_set.contains(INTEGER)) column_type = hive::model::ColumnType::Integer;
            if (flags_set.contains(REAL)) column_type = hive::model::ColumnType::Real;
            if (flags_set.contains(BLOB)) column_type = hive::model::ColumnType::Blob;
            if (flags_set.contains(BOOL)) column_type = hive::model::ColumnType::Bool;
            if (flags_set.contains(DATETIME)) column_type = hive::model::ColumnType::DateTime;

            return *this;
        }

    public:
    };
}