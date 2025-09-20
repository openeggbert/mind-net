//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/api/Plugin.h"

#include "mindnet/impl/sqlite/RepositoryImplSqlite.h"

namespace mindnet::api
{
    Plugin::Plugin(const std::string& name_, const std::string& description_,
                   std::vector<string> apps_,
                   const std::vector<std::string>& plugins_which_this_plugin_depends_on_)
        : name(name_), description(description_),
          apps(std::move(apps_)),
          plugins_which_this_plugin_depends_on(plugins_which_this_plugin_depends_on_)
    {
        if (get_name() != "core")
        {
            plugins_which_this_plugin_depends_on.emplace_back("core");
        }
    }

    // ----------------------------
    // Getters
    // ----------------------------
    const std::string& Plugin::get_name() const
    {
        return name;
    }

    const std::string& Plugin::get_description() const
    {
        return description;
    }

    const std::vector<std::string>& Plugin::depends_on_plugins() const
    {
        return plugins_which_this_plugin_depends_on;
    }

    std::vector<string> Plugin::get_apps() const
    {
        return apps;
    }

    bool Plugin::is_closed_for_changes() const
    {
        return is_closed_for_changes_;
    }

    // ----------------------------
    // Actions
    // ----------------------------
    void Plugin::close_for_changes()
    {
        if (is_closed_for_changes_) return;
        is_closed_for_changes_ = true;
    }

    void Plugin::register_model(
        model::ModelDefinition& model_definition,
        const std::shared_ptr<IValidator>& validator
        )
    {
        if (is_closed_for_changes_)
        {
            warn << "Plugin " << name << " is closed for changes" << commit;
            return;
        }

        std::shared_ptr<IRepository> repository = std::make_shared<impl::sqlite::RepositoryImplSqlite>(
            model_definition);

        auto registration = std::make_shared<ModelRegistration>(
            model_definition,
            validator,
            repository
            );

        model_registrations.push_back(registration);
    }

    void Plugin::register_trigger(const TriggerPtr& trigger)
    {
        return triggers.push_back(trigger);
    }

    void Plugin::register_migrations(const MigrationScriptsPtr& migration_scripts)
    {
        migration_scripts_ = migration_scripts;
    }

    MigrationScriptsPtr Plugin::get_migration_scripts() const
    {
        return migration_scripts_;
    }

    void Plugin::destroy_migration_scripts()
    {
        migration_scripts_.reset();
    }

    const std::vector<TriggerPtr>& Plugin::get_triggers() const
    {
        return triggers;
    }

    const std::vector<std::shared_ptr<ModelRegistration>>& Plugin::get_model_registrations() const
    {
        return model_registrations;
    }
} // namespace mindnet::api
