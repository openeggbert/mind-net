//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/api/Plugin.h"

#include "mindnet/api/RepositoryFactory.h"
#include "mindnet/essential/Global.h"

namespace mindnet::api
{
    using_loggers()

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

    std::set<string> registered_models;
    void Plugin::register_model(
        model::ModelDefinition& model_definition,
        const std::shared_ptr<IValidator>& validator,
        std::shared_ptr<RepositoryFactory>& repository_factory
        )
    {
        if (is_closed_for_changes_)
        {
            warn << "Plugin " << name << " is closed for changes" << commit;
            return;
        }

        auto& model_name = model_definition.get_model_name();
        if (registered_models.contains(model_name))
        {
            warn << "Model with name " << model_name << " is already registered" << commit;
            return;
        }
        registered_models.insert(model_name);
        std::shared_ptr<IRepository> repository = repository_factory->create(model_definition);

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
        essential::DatabaseType current_database_type = essential::g_configuration.database_type;
        if (migration_scripts.get()->get_database_type() != current_database_type)
        {
            return;
        }
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
