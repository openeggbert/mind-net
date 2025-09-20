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
#include "../plugins/core/enums/Crudl.h"
#include "BaseColumns.h"

namespace mindnet::model
{
    using std::string;

    /**
     * Represents the definition of a database model including its name,
     * auto-increment status, columns and allowed CRUD operations.
     */
    struct ModelDefinition
    {
    private:
        string model_name; ///< Name of the model
        string group;
        int group_order_index = 0;
        column_definitions columns; ///< Column definitions for the model
        std::set<plugins::core::enums::Crudl> allowed_rest_operations; ///< Allowed CRUD operations for REST API
        bool virtual_table = false;
        std::vector<CustomAction> custom_actions{};
        std::string title_column;

    public:
        /**
         * Constructs a ModelDefinition with the given name
         * @param name The name for the model
         */
        ModelDefinition(const string& name)
        {
            model_name = name;
        };

        // Getters
        /** @return The model name */
        [[nodiscard]] const string& get_model_name() const
        {
            return model_name;
        }

        [[nodiscard]] const string& get_group() const
        {
            return group;
        }

        [[nodiscard]] int get_group_order_index() const
        {
            return group_order_index;
        }

        /** @return The column definitions */
        [[nodiscard]] const column_definitions& get_columns() const
        {
            return columns;
        }

        /** @return The allowed CRUD operations */
        [[nodiscard]] const std::set<plugins::core::enums::Crudl>& get_allowed_rest_operations() const
        {
            return allowed_rest_operations;
        }

        /** @return True if the model is virtual table otherwise false. */
        [[nodiscard]] const bool is_virtual_table() const
        {
            return virtual_table;
        }

        [[nodiscard]] const std::vector<CustomAction> get_custom_actions() const
        {
            return custom_actions;
        }
        const std::string& get_title_column() const
        {
            return title_column;
        }
        //Setters
        ModelDefinition& set_group(const string& group_, const int group_order_index_ = 0)
        {
            group = group_;
            group_order_index = group_order_index_;
            return *this;
        }

        /**
         * Sets the column definitions and adds standard base columns
         * @param cols Column definitions to set
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_columns(column_definitions cols)
        {
            cols.emplace(cols.begin(), BaseColumns::UPDATED_AT);
            cols.emplace(cols.begin(), BaseColumns::CREATED_AT);
            cols.emplace(cols.begin(), BaseColumns::ID);
            columns = std::move(cols);

            return *this;
        }

        /**
         * Sets the allowed CRUD operations
         * @param ops Set of allowed operations
         * @return Reference to this object for method chaining
         */
        ModelDefinition& set_rest_operations(std::set<plugins::core::enums::Crudl> ops)
        {
            if (!allowed_rest_operations.empty())
            {
                throw std::invalid_argument("allowed_rest_operations were already set");
            }
            allowed_rest_operations = std::move(ops);
            return *this;
        }

        ModelDefinition& set_rest_operations(std::string ops_as_string)
        {
            if (!allowed_rest_operations.empty())
            {
                throw std::invalid_argument("allowed_rest_operations were already set");
            }

            if (ops_as_string.empty())
            {
                allowed_rest_operations.clear();
                return *this;
            }
            EnumDefinition enumDef = plugins::core::enums::crudl_to_enum_definition();
            if (ops_as_string.size() > enumDef.get_value_count())
            {
                throw std::invalid_argument("Invalid value for allowed_rest_operations");
            }
            for (char ch : ops_as_string)
            {
                plugins::core::enums::Crudl crudl = plugins::core::enums::char_to_crudl(ch);
                if (crudl == plugins::core::enums::Crudl::Undefined)
                {
                    throw std::invalid_argument("Invalid value for allowed_rest_operations: ");
                }
                allowed_rest_operations.insert(crudl);
            }

            return *this;
        }

        ModelDefinition& set_rest_operations(const char* ops_as_string)
        {
            return set_rest_operations(std::string(ops_as_string));
        }

        ModelDefinition& set_all_rest_operations()
        {
            return set_rest_operations(plugins::core::enums::CRUDL);
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
        ModelDefinition& set_title_column(std::string title_column_)
        {
            title_column = title_column_;
            return *this;
        }

        [[nodiscard]] ModelDefinition& add_custom_action(
            plugins::core::enums::Crudl crudl_,
            std::string& model_name_,
            std::string& label_,
            const std::vector<std::string>& params_ = {})
        {
            if (params_.size() != 2)
            {
                throw std::invalid_argument("Invalid params for custom action");
            }
            string_map map;
            for (int i = 0; i < params_.size(); i++)
            {
                auto param1 = params_.at(i);
                i++;
                auto param2 = params_.at(i);
                map.insert(std::make_pair(param1, param2));
            }
            auto custom_action = CustomAction(crudl_, model_name_, label_, map);
            custom_actions.emplace_back(custom_action);

            return *this;
        }

        [[nodiscard]] ModelDefinition& add_custom_list_action(
            std::string model_name_,
            std::string label_,
            const std::vector<std::string>& params_)
        {
            return add_custom_action(plugins::core::enums::Crudl::List, model_name_, label_, params_);
        }

        [[nodiscard]] ModelDefinition& add_custom_create_action(
            std::string model_name_,
            std::string label_,
            const std::vector<std::string>& params_)
        {
            return add_custom_action(plugins::core::enums::Crudl::Create, model_name_, label_, params_);
        }
    };
}

#endif // MODELDEFINITION_H
