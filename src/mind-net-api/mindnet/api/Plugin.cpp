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

#include "mindnet/api/Plugin.hpp"

#include "mindnet/api/RepositoryFactory.hpp"
#include "mindnet/essential/Global.hpp"

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

    std::vector<string> Plugin::get_library_files() const
    {
        return library_files;
    }

    void Plugin::register_library_file(const string library_file)
    {
        if (is_closed_for_changes_)
        {
            warn << "Plugin " << name << " is closed for changes" << commit;
            return;
        }

        library_files.push_back(library_file);
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
        const model::ModelDefinition& model_definition,
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

    void Plugin::register_query(const QueryPtr& query)
    {
        essential::DatabaseType current_database_type = essential::g_configuration.database_type;
        if (query.get()->get_database_type() != current_database_type)
        {
            return;
        }
        return queries.push_back(query);
    }

    void Plugin::register_job(const JobPtr& job)
    {
        return jobs.push_back(job);
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

    const std::vector<QueryPtr>& Plugin::get_queries() const
    {
        return queries;
    }

    const std::vector<JobPtr>& Plugin::get_jobs() const
    {
        return jobs;
    }

    const std::vector<std::shared_ptr<ModelRegistration>>& Plugin::get_model_registrations() const
    {
        return model_registrations;
    }
} // namespace mindnet::api
