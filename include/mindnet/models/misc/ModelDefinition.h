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
#ifndef MODELDEFINITION_H
#define MODELDEFINITION_H

#include <set>

#include "ostream"
#include <string>

#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"

namespace mindnet::models::misc
{
    using std::string;

    /**
     * Represents the definition of a database model including its name,
     * auto-increment status, columns and allowed CRUD operations.
     */
    struct ModelDefinition
    {
    private:
        str model_name; ///< Name of the model
        bool auto_increment = true; ///< Whether the model uses auto-incrementing IDs
        column_definitions columns; ///< Column definitions for the model
        std::set<enums::Crudl> allowed_crudl_rest_operations; ///< Allowed CRUD operations for REST API
        bool virtual_table = false;

    public:
        /**
         * Constructs a ModelDefinition with the given name
         * @param name The name for the model
         */
        ModelDefinition(const str& name)
        {
            model_name = name;
        };

        // Getters
        /** @return The model name */
        [[nodiscard]] const str& get_model_name() const
        {
            return model_name;
        }

        /** @return Whether auto-increment is enabled */
        [[nodiscard]] const bool get_auto_increment() const
        {
            return auto_increment;
        }

        /** @return The column definitions */
        [[nodiscard]] const column_definitions& get_columns() const
        {
            return columns;
        }

        /** @return The allowed CRUD operations */
        [[nodiscard]] const std::set<enums::Crudl>& get_allowed_crudl_rest_operations() const
        {
            return allowed_crudl_rest_operations;
        }

        /** @return True if the model is virtual table otherwise false. */
        [[nodiscard]] const bool get_virtual_table() const
        {
            return virtual_table;
        }

        // Setters
        /**
         * Sets the model name
         * @param name New model name
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_name(str name)
        {
            model_name = name;
            return *this;
        }

        /**
         * Sets the auto-increment flag
         * @param value True to enable auto-increment
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_auto_inc(bool value)
        {
            auto_increment = value;
            return *this;
        }

        /**
         * Sets the column definitions and adds standard base columns
         * @param cols Column definitions to set
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_columns(column_definitions cols)
        {
            cols.emplace(cols.begin(), columns::BaseColumns::UPDATED_AT);
            cols.emplace(cols.begin(), columns::BaseColumns::CREATED_AT);
            cols.emplace(cols.begin(), columns::BaseColumns::ID);
            columns = std::move(cols);

            return *this;
        }

        /**
         * Sets the allowed CRUD operations
         * @param ops Set of allowed operations
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_operations(std::set<enums::Crudl> ops)
        {
            allowed_crudl_rest_operations = std::move(ops);
            return *this;
        }

        /**
         * Configures the virtual table for a database model.
         *
         * @param value Boolean.
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_virtual_table(bool value)
        {
            virtual_table = value;
            return *this;
        }
    };
}

#endif // MODELDEFINITION_H
