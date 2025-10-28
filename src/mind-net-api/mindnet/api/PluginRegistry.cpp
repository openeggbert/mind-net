//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/api/PluginRegistry.h"
#include <string>

#include "mindnet/essential/Global.h"
#include "mindnet/api/Plugin.h"

namespace mindnet::api
{
    using_loggers()
    static std::unordered_set<std::string> unique_names_for_apps;

    bool PluginRegistry::has_plugin_name(const std::string& plugin_name) const
    {
        return plugins.count(plugin_name);
    };

    std::vector<std::string> PluginRegistry::get_plugin_names() const
    {
        std::vector<std::string> keys;
        for (auto& p : plugins)
        {
            keys.push_back(p.first);
        }
        return keys;
    }

    std::vector<std::string> PluginRegistry::get_plugin_names_sorted_by_dependencies() const
    {
        std::unordered_map<std::string, PluginPtr> plugin_map;
        for (auto& p : plugins)
        {
            plugin_map[p.first] = p.second;
        }

        std::unordered_map<std::string, int> visit_state; // 0=unvisited,1=visiting,2=visited
        std::vector<std::string> sorted;

        std::function<void(const std::string&)> dfs = [&](const std::string& name)
        {
            if (visit_state[name] == 1)
            {
                throw CyclicDependencyException("Cyclic dependency detected at plugin: " + name);
            }
            if (visit_state[name] == 2) return;

            visit_state[name] = 1;
            auto plugin = plugin_map.at(name); // must exist, otherwise error elsewhere
            for (auto& dep : plugin->depends_on_plugins())
            {
                if (plugin_map.find(dep) == plugin_map.end())
                {
                    throw MissingDependencyException(
                        "Plugin '" + name + "' depends on missing plugin: " += dep
                    );
                }
                dfs(dep);
            }
            visit_state[name] = 2;
            sorted.push_back(name);
        };

        for (auto& p : plugins)
        {
            if (visit_state[p.first] == 0)
            {
                dfs(p.first);
            }
        }

        return sorted;
    }


    PluginPtr PluginRegistry::get_plugin(const std::string& plugin_name) const
    {
        if (!has_plugin_name(plugin_name)) return nullptr;
        return plugins.at(plugin_name);
    };

    void PluginRegistry::register_plugin(const PluginPtr& plugin)
    {
        if (plugins.empty() && plugin->get_name() != "core")
        {
            throw std::runtime_error(
                "Plugin 'core' must be registered first, but found plugin '" + plugin->get_name() + "' instead"
            );
        }
        if (plugin->get_name() != "core" && !essential::g_configuration.allowed_plugins.contains(plugin->get_name()))
        {
            warn << "Plugin " << plugin->get_name() << " is not allowed. This plugin won't be registered." << commit;
            return;
        }
        if (!plugin->is_closed_for_changes())
        {
            err << "Plugin " << plugin->get_name() << " is not closed for changes and cannot be registered" << commit;
            throw std::runtime_error("Plugin is not closed for changes and cannot be registered");
        }

        if (has_plugin_name(plugin->get_name()))
        {
            warn << "Plugin " << plugin->get_name() << " already registered" << commit;
            return;
        }
        for (auto& app : plugin->get_apps())
        {
            if (unique_names_for_apps.contains(app))
            {
                throw std::runtime_error("App with name " + app + " was already registered.");
            }
            unique_names_for_apps.insert(app);
        }
        plugins[plugin->get_name()] = plugin;
    };

    int PluginRegistry::get_plugin_count() const
    {
        return plugins.size();
    };
}
