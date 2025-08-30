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
#ifndef COLUMNDEFINITION_H
#define COLUMNDEFINITION_H

#include <optional>
#include <string>
#include "EnumDefinition.h"
#include "mindnet/persistence/impl/sqlite/repositories/ContentRepositoryImplSqlite.h"
#define using_flags()\
using misc::NOT_NULL;\
using misc::UNIQUE;\
using misc::TEXT;\
using misc::TEXTAREA;\
using misc::INTEGER;\
using misc::REAL;\
using misc::BLOB;\
using misc::BOOL;\
using misc::DATETIME;

namespace mindnet::models::misc
{
    enum ColumnDefinitionFlag
    {
        NOT_NULL = 1 << 0,
        UNIQUE = 1 << 1,
        TEXT = 1 << 2,
        TEXTAREA = 1 << 3,
        INTEGER = 1 << 4,
        REAL = 1 << 5,
        BLOB = 1 << 6,
        BOOL = 1 << 7,
        DATETIME = 1 << 8
    };

    inline std::vector<ColumnDefinitionFlag> column_definition_flag_values()
    {
        return {
            NOT_NULL,
            UNIQUE,
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
        enums::ColumnType column_type = enums::ColumnType::TEXT;
        bool mandatory = false;
        bool primary_key = false;
        std::string foreign_key;
        std::optional<EnumDefinition> enum_definition;
        bool unique = false;
        std::string default_value;

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
            using bc = columns::BaseColumns;
            if (ends_with_id(column_name))
            {
                column_type = mindnet::enums::ColumnType::INTEGER;
            }

            if (column_name == bc::ID)
            {
                primary_key = true;
                mandatory = true;
                column_type = mindnet::enums::ColumnType::INTEGER;
            }
            if (column_name == bc::CREATED_AT)
            {
                mandatory = true;
                column_type = mindnet::enums::ColumnType::DATETIME;
            }
            if (column_name == bc::UPDATED_AT)
            {
                column_type = mindnet::enums::ColumnType::DATETIME;
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
        [[nodiscard]] const mindnet::enums::ColumnType get_column_type() const
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

        /** @return The enum definition */
        [[nodiscard]] const std::optional<mindnet::models::misc::EnumDefinition>& get_enum_definition() const
        {
            return enum_definition;
        }


        [[nodiscard]] const bool is_unique() const
        {
            return unique;
        }

        [[nodiscard]] const std::string get_default_value() const
        {
            return default_value;
        }

        // Setters


        /**
     * Sets the enum definition
     * @param value New enum definition
     * @return Reference to this object for method chaining
     */
        ColumnDefinition& set_enum_definition(std::optional<mindnet::models::misc::EnumDefinition> value)
        {
            enum_definition = value;
            column_type = mindnet::enums::ColumnType::INTEGER;
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
            return *this;
        }

        ColumnDefinition& set_default_value(std::string value)
        {
            default_value = value;
            return *this;
        }

    private:
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
            if (flags_set.contains(NOT_NULL)) mandatory = true;
            if (flags_set.contains(UNIQUE)) unique = true;
            //
            if (flags_set.contains(TEXT)) column_type = enums::ColumnType::TEXT;
            if (flags_set.contains(TEXTAREA)) column_type = enums::ColumnType::TEXTAREA;
            if (flags_set.contains(INTEGER)) column_type = enums::ColumnType::INTEGER;
            if (flags_set.contains(REAL)) column_type = enums::ColumnType::REAL;
            if (flags_set.contains(BLOB)) column_type = enums::ColumnType::BLOB;
            if (flags_set.contains(BOOL)) column_type = enums::ColumnType::BOOL;
            if (flags_set.contains(DATETIME)) column_type = enums::ColumnType::DATETIME;

            return *this;
        }
    public:
    };
}


#endif // COLUMNDEFINITION_H
