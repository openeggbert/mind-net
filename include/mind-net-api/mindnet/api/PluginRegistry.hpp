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
#include <string>
#include <vector>

#include "Plugin.hpp"
#include "IValidator.hpp"


namespace mindnet::api
{
    struct CyclicDependencyException : std::runtime_error
    {
        explicit CyclicDependencyException(const std::string& msg) : std::runtime_error(msg)
        {
        }
    };

    struct MissingDependencyException : std::runtime_error
    {
        explicit MissingDependencyException(const std::string& message)
            : std::runtime_error(message)
        {
        }
    };


    class PluginRegistry
    {
    public:
        PluginRegistry() = default;
        ~PluginRegistry() = default;

        bool has_plugin_name(const std::string& plugin_name) const;
        [[nodiscard]] std::vector<std::string> get_plugin_names() const;
        std::vector<std::string> get_plugin_names_sorted_by_dependencies() const;
        [[nodiscard]] PluginPtr get_plugin(const std::string& plugin_name) const;
        void register_plugin(const PluginPtr& plugin);
        [[nodiscard]] int get_plugin_count() const;

    private:
        std::map<std::string, PluginPtr> plugins;
    };

    typedef std::shared_ptr<PluginRegistry> PluginRegistryPtr;
}
