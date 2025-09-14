//
// Created by robertvokac on 9/7/25.
//
#ifndef PLUGIN_H
#define PLUGIN_H
#include <string>
#include <vector>

#include "IValidator.h"
#include "ModelRegistration.h"
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
            ModelDefinition& model_definition,
            std::shared_ptr<IValidator> validator,
            api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer);
        const std::vector<std::shared_ptr<ModelRegistration>>& get_model_registrations() const;

    private:
        std::string name;
        std::string description;
        std::vector<string> plugins_which_this_plugin_depends_on;
        bool has_app_ = false;
        bool is_closed_for_changes_ = false;
        std::vector<std::shared_ptr<ModelRegistration>> model_registrations;
    };

    typedef std::shared_ptr<Plugin> PluginPtr;
}
#endif // PLUGIN_H
