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

#include <set>

#include "ostream"
#include <string>

#include "mindnet/essential/DatabaseType.hpp"
#include "BaseColumns.hpp"
#include "CustomAction.hpp"
#include "mindnet/model/ModelTypeDefs.hpp"

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
        string plugin_name;
        string group;
        int group_order_index = 0;
        column_definitions columns; ///< Column definitions for the model
        std::set<mindnet::essential::Crudl> allowed_rest_operations; ///< Allowed CRUD operations for REST API
        bool virtual_table = false;
        bool no_table = false;
        std::vector<CustomAction> custom_actions{};
        std::string title_column;
        bool reader_can_write{false};
        bool cache_enabled{true};
        bool cached_after_create{true};

    public:
        /**
         * Constructs a ModelDefinition with the given name
         * @param name The name for the model
         * @param plugin_name_
         */
        explicit ModelDefinition(const string& name, const string& plugin_name_)
        {
            model_name = name;
            plugin_name = plugin_name_;
        }

        // Getters
        /** @return The model name */
        [[nodiscard]] const string& get_model_name() const
        {
            return model_name;
        }

        /** @return The plugin name */
        [[nodiscard]] const string& get_plugin_name() const
        {
            return plugin_name;
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
        [[nodiscard]] const std::set<mindnet::essential::Crudl>& get_allowed_rest_operations() const
        {
            return allowed_rest_operations;
        }

        /** @return True if the model is virtual table otherwise false. */
        [[nodiscard]] const bool is_virtual_table() const
        {
            return virtual_table;
        }

        /** @return True if the model is no table otherwise false. */
        [[nodiscard]] const bool is_no_table() const
        {
            return no_table;
        }

        [[nodiscard]] const std::vector<CustomAction> get_custom_actions() const
        {
            return custom_actions;
        }

        const std::string& get_title_column() const
        {
            return title_column;
        }

        const bool is_reader_can_write() const
        {
            return reader_can_write;
        }

        [[nodiscard]] bool is_cache_enabled() const
        {
            return cache_enabled;
        }

        [[nodiscard]] bool is_cached_after_create() const
        {
            return cached_after_create;
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
        ModelDefinition& set_rest_operations(std::set<mindnet::essential::Crudl> ops)
        {
            if (!allowed_rest_operations.empty())
            {
                throw std::invalid_argument(
                    "allowed_rest_operations were already set for model definition " + model_name);
            }
            allowed_rest_operations = std::move(ops);
            return *this;
        }

        ModelDefinition& set_rest_operations(std::string ops_as_string)
        {
            if (!allowed_rest_operations.empty())
            {
                throw std::invalid_argument(
                    "allowed_rest_operations were already set for model definition " + model_name);
            }

            if (ops_as_string.empty())
            {
                allowed_rest_operations.clear();
                return *this;
            }

            if (ops_as_string.size() > essential::CRUDL_VALUE_COUNT)
            {
                throw std::invalid_argument("Invalid value for allowed_rest_operations");
            }
            for (char ch : ops_as_string)
            {
                mindnet::essential::Crudl crudl = essential::char_to_crudl(ch);
                if (crudl == mindnet::essential::Crudl::Undefined)
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
            return set_rest_operations(essential::CRUDL);
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

        ModelDefinition& set_no_table(bool value)
        {
            no_table = value;
            return *this;
        }

        ModelDefinition& set_title_column(std::string title_column_)
        {
            title_column = title_column_;
            return *this;
        }

        ModelDefinition& allow_reader_write()
        {
            reader_can_write = true;
            return *this;
        }

        ModelDefinition& set_cache_enabled(bool value)
        {
            cache_enabled = value;
            return *this;
        }

        ModelDefinition& set_cached_after_create(bool value)
        {
            cached_after_create = value;
            return *this;
        }

        [[nodiscard]] ModelDefinition& add_custom_action(
            mindnet::essential::Crudl crudl_,
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
            return add_custom_action(mindnet::essential::Crudl::List, model_name_, label_, params_);
        }

        [[nodiscard]] ModelDefinition& add_custom_create_action(
            std::string model_name_,
            std::string label_,
            const std::vector<std::string>& params_)
        {
            return add_custom_action(mindnet::essential::Crudl::Create, model_name_, label_, params_);
        }

        [[nodiscard]] ModelDefinition& add_custom_read_action(
            std::string model_name_,
            std::string label_,
            const std::vector<std::string>& params_)
        {
            return add_custom_action(mindnet::essential::Crudl::Read, model_name_, label_, params_);
        }
    };
}