//
// Created by robertvokac on 9/7/25.
//
#ifndef PLUGIN_H
#define PLUGIN_H
#include <string>
#include <vector>

#include "MigrationScripts.h"
#include "ModelRegistration.h"
#include "Trigger.h"
#include "jwt-cpp/jwt.h"
#include "mindnet/impl/sqlite/RepositoryHelper.h"

namespace mindnet::api
{
    class Plugin
    {
    public:
        Plugin(
            const std::string& name_,
            const std::string& description_,
            const std::vector<std::string>& plugins_which_this_plugin_depends_on_,
            bool has_app_);
        ~Plugin() = default;

        const std::string& get_name() const;
        const std::string& get_description() const;
        const std::vector<std::string>& depends_on_plugins() const;
        bool has_app() const;
        void close_for_changes();
        bool is_closed_for_changes() const;
        void register_model(
            model::ModelDefinition& model_definition,
            const std::shared_ptr<IValidator>& validator);
        void register_trigger(const TriggerPtr& trigger);
        void register_migrations(const MigrationScriptsPtr& migration_scripts);
        [[nodiscard]] MigrationScriptsPtr get_migration_scripts() const;
        void destroy_migration_scripts();
        const std::vector<TriggerPtr>& get_triggers() const;
        const std::vector<std::shared_ptr<ModelRegistration>>& get_model_registrations() const;

    private:
        std::string name;
        std::string description;
        std::vector<string> plugins_which_this_plugin_depends_on;
        bool has_app_ = false;
        bool is_closed_for_changes_ = false;
        std::vector<std::shared_ptr<ModelRegistration>> model_registrations;
        std::vector<TriggerPtr> triggers;
        MigrationScriptsPtr migration_scripts_ = nullptr;
    };

    typedef std::shared_ptr<Plugin> PluginPtr;
}
#endif // PLUGIN_H
