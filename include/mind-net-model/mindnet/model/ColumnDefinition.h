///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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
#ifndef COLUMNDEFINITION_H
#define COLUMNDEFINITION_H

#include <cstring>
#include <optional>
#include <string>
#include <vector>
#include <set>

#include "BaseColumns.h"
#include "ColumnType.h"
#include "CustomAction.h"
#include "EnumDefinition.h"

#define using_flags()\
using mindnet::model::MANDATORY;\
using mindnet::model::UNIQUE;\
using mindnet::model::FOREIGN_KEY;\
using mindnet::model::AUTO;\
using mindnet::model::HIDDEN;\
using mindnet::model::READONLY;\
using mindnet::model::TEXT;\
using mindnet::model::TEXTAREA;\
using mindnet::model::INTEGER;\
using mindnet::model::REAL;\
using mindnet::model::BLOB;\
using mindnet::model::BOOL;\
using mindnet::model::DATETIME;

namespace mindnet::model
{
    enum ColumnDefinitionFlag
    {
        MANDATORY = 1 << 0,
        UNIQUE = 1 << 1,
        FOREIGN_KEY = 1 << 2,
        AUTO = 1 << 3,
        HIDDEN = 1 << 4,
        READONLY = 1 << 5,
        TEXT = 1 << 6,
        TEXTAREA = 1 << 7,
        INTEGER = 1 << 8,
        REAL = 1 << 9,
        BLOB = 1 << 10,
        BOOL = 1 << 11,
        DATETIME = 1 << 12
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
                column_type = mindnet::model::ColumnType::Integer;
            }

            if (column_name == bc::ID)
            {
                primary_key = true;
                mandatory = true;
                unique = true;
                auto_ = true;
                column_type = mindnet::model::ColumnType::Integer;
                description = "Unique identifier of the record.";
                readonly = true;
            }
            if (column_name == bc::CREATED_AT)
            {
                mandatory = true;
                auto_ = true;
                column_type = mindnet::model::ColumnType::DateTime;
                description = "Timestamp when the record was created.";
                readonly = true;
            }
            if (column_name == bc::UPDATED_AT)
            {
                auto_ = true;
                column_type = mindnet::model::ColumnType::DateTime;
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
        [[nodiscard]] const mindnet::model::ColumnType get_column_type() const
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
        [[nodiscard]] const int get_default_int_value() const
        {
            return default_value.empty() ? 0 : std::stoi(default_value);
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
            column_type = mindnet::model::ColumnType::Integer;
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
            column_type = mindnet::model::ColumnType::Integer;
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
            //
            if (flags_set.contains(TEXT)) column_type = mindnet::model::ColumnType::Text;
            if (flags_set.contains(TEXTAREA)) column_type = mindnet::model::ColumnType::TextArea;
            if (flags_set.contains(INTEGER)) column_type = mindnet::model::ColumnType::Integer;
            if (flags_set.contains(REAL)) column_type = mindnet::model::ColumnType::Real;
            if (flags_set.contains(BLOB)) column_type = mindnet::model::ColumnType::Blob;
            if (flags_set.contains(BOOL)) column_type = mindnet::model::ColumnType::Bool;
            if (flags_set.contains(DATETIME)) column_type = mindnet::model::ColumnType::DateTime;

            return *this;
        }

    public:
    };
}

#endif // COLUMNDEFINITION_H
