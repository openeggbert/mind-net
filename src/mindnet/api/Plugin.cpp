//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/api/Plugin.h"

#include "mindnet/impl/sqlite/RepositoryImplSqlite.h"

namespace mindnet::api
{
    Plugin::Plugin(const std::string& name, const std::string& description,
                   const std::vector<std::string>& plugins_which_this_plugin_depends_on, bool has_app)
        : name(name), description(description),
          plugins_which_this_plugin_depends_on(plugins_which_this_plugin_depends_on), has_app_(has_app)
    {
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

    bool Plugin::has_app() const
    {
        return has_app_;
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
        ModelDefinition& model_definition,
        std::shared_ptr<IValidator> validator,
        api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer)
    {
        if (is_closed_for_changes_)
        {
            warn << "Plugin " << name << " is closed for changes" << commit;
            return;
        }

        std::shared_ptr<IRepository> repository = std::make_shared<impl::sqlite::RepositoryImplSqlite>(
        convert_rest_request_to_entity_fields_pointer,
            model_definition);

        auto registration = std::make_shared<ModelRegistration>(
            model_definition,
            validator,
            convert_rest_request_to_entity_fields_pointer,
            repository
            );

        model_registrations.push_back(registration);
    }

    const std::vector<std::shared_ptr<ModelRegistration>>& Plugin::get_model_registrations() const
    {
        return model_registrations;
    }
} // namespace mindnet::api
