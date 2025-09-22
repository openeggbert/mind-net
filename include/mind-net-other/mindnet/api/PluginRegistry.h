//
// Created by robertvokac on 9/7/25.
//
#ifndef PLUGINREGISTRY_H
#define PLUGINREGISTRY_H

#include <string>
#include <vector>

#include "Plugin.h"
#include "IValidator.h"
#include "jwt-cpp/jwt.h"

namespace mindnet::api
{
    struct CyclicDependencyException : std::runtime_error {
        explicit CyclicDependencyException(const std::string& msg) : std::runtime_error(msg) {}
    };

    struct MissingDependencyException : std::runtime_error {
        explicit MissingDependencyException(const std::string& message)
            : std::runtime_error(message) {}
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
#endif // PLUGINREGISTRY_H
