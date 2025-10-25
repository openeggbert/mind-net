//
// Created by robertvokac on 9/7/25.
//
#ifndef PLUGIN_H
#define PLUGIN_H
#include <string>
#include <vector>

#include "MigrationScripts.h"
#include "ModelRegistration.h"
#include "Query.h"
#include "Trigger.h"

namespace mindnet::model
{
    struct ModelDefinition;
}

namespace mindnet::api
{
    class RepositoryFactory;

    class Plugin
    {
    public:
        Plugin(
            const std::string& name_,
            const std::string& description_,
            std::vector<string> apps_ = {},
            const std::vector<std::string>& plugins_which_this_plugin_depends_on_ = {}
            );
        ~Plugin() = default;

        [[nodiscard]] const std::string& get_name() const;
        [[nodiscard]] const std::string& get_description() const;
        [[nodiscard]] const std::vector<std::string>& depends_on_plugins() const;
        [[nodiscard]] std::vector<string> get_apps() const;
        [[nodiscard]] std::vector<string> get_library_files() const;
        void register_library_file(string library_file);
        void close_for_changes();
        [[nodiscard]] bool is_closed_for_changes() const;
        void register_model(
            model::ModelDefinition& model_definition,
            const std::shared_ptr<IValidator>& validator,
            std::shared_ptr<RepositoryFactory>& repository_factory);
        void register_trigger(const TriggerPtr& trigger);
        void register_query(const QueryPtr& query);
        void register_migrations(const MigrationScriptsPtr& migration_scripts);
        [[nodiscard]] MigrationScriptsPtr get_migration_scripts() const;
        void destroy_migration_scripts();
        [[nodiscard]] const std::vector<TriggerPtr>& get_triggers() const;
        [[nodiscard]] const std::vector<QueryPtr>& get_queries() const;
        [[nodiscard]] const std::vector<std::shared_ptr<ModelRegistration>>& get_model_registrations() const;

    private:
        std::string name;
        std::string description;
        std::vector<string> plugins_which_this_plugin_depends_on;
        std::vector<string> apps;
        std::vector<string> library_files;
        bool is_closed_for_changes_ = false;
        std::vector<std::shared_ptr<ModelRegistration>> model_registrations;
        std::vector<TriggerPtr> triggers;
        std::vector<QueryPtr> queries;
        MigrationScriptsPtr migration_scripts_ = nullptr;
    };

    typedef std::shared_ptr<Plugin> PluginPtr;
}
#endif // PLUGIN_H
