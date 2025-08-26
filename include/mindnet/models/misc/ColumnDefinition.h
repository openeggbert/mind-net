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

namespace mindnet::models::misc
{
    struct ColumnDefinition
    {
    private:
        std::string column_name;
        enums::ColumnType column_type;
        bool mandatory = false;
        bool primary_key = false;
        std::string foreign_key;
        std::optional<EnumDefinition> enum_definition;
        bool unique = false;

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
        ColumnDefinition(const char* name, mindnet::enums::ColumnType type = mindnet::enums::ColumnType::TEXT)
        {
            column_name = name;
            column_type = type;
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
        [[nodiscard]] const bool get_mandatory() const
        {
            return mandatory;
        }

        /** @return Whether the column is a primary key */
        [[nodiscard]] const bool get_primary_key() const
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


        [[nodiscard]] const bool get_unique() const
        {
            return unique;
        }

        // Setters
        /**
         * Sets the column name
         * @param name New column name
         * @return Reference to this object for method chaining
         */
        ColumnDefinition& set_column_name(std::string name)
        {
            column_name = name;
            return *this;
        }

        /**
         * Sets the column type
         * @param type New column type
         * @return Reference to this object for method chaining
         */
        ColumnDefinition& set_column_type(mindnet::enums::ColumnType type)
        {
            column_type = type;
            return *this;
        }

        /**
 * Helper method to set column type to INTEGER
 * @return Reference to this object for method chaining
 */
        ColumnDefinition& integer()
        {
            set_column_type(mindnet::enums::ColumnType::INTEGER);
            return *this;
        }

        ColumnDefinition& datetime()
        {
            set_column_type(mindnet::enums::ColumnType::DATETIME);
            return *this;
        }

        ColumnDefinition& textarea()
        {
            set_column_type(mindnet::enums::ColumnType::TEXTAREA);
            return *this;
        }
        ColumnDefinition& bool_column()
        {
            set_column_type(mindnet::enums::ColumnType::BLOB);
            return *this;
        }


        /**
         * Sets whether the column is mandatory
         * @param value True to make mandatory
         * @return Reference to this object for method chaining
         */
        ColumnDefinition& set_mandatory(bool value)
        {
            mandatory = value;
            return *this;
        }
        ColumnDefinition& set_mandatory()
        {
            set_mandatory(true);
            return *this;
        }


        /**
     * Sets the enum definition
     * @param value New enum definition
     * @return Reference to this object for method chaining
     */
        ColumnDefinition& set_enum_definition(std::optional<mindnet::models::misc::EnumDefinition> value)
        {
            enum_definition = value;
            integer();
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
        ColumnDefinition& set_unique(bool value)
        {
            unique = value;
            return *this;
        }
        ColumnDefinition& set_unique()
        {
            set_unique(true);
            return *this;
        }
    };
}


#endif // COLUMNDEFINITION_H
