//
// Created by robertvokac on 9/13/25.
//

#include "mindnet/Service.h"
//
#define add_validator(plugin, model, Model)\
api::IValidator* model##_validator = new mindnet::plugins:: plugin ::validators:: Model##Validator();\
validators[#model] = model##_validator;

namespace mindnet
{
    static constexpr bool VALIDATION_ENABLED = false;
    using validator = api::IValidator*;
    using mindnet::OperationResult;
    using model::ModelDefinition;
    using plugins::core::enums::Crudl;

    Service::Service(
        const DbPtr& db_,
        const api::PluginRegistryPtr& plugin_registry_ptr_
        ) :
    IService(db_),
    db_ptr(db_),
    plugin_registry_ptr(plugin_registry_ptr_),
    trigger_registry_ptr(std::make_shared<api::TriggerRegistry>())
    {
        for (auto& plugin_name : plugin_registry_ptr->get_plugin_names_sorted_by_dependencies())
        {
            auto plugin = plugin_registry_ptr->get_plugin(plugin_name);
            if (VALIDATION_ENABLED)
            {
                for (auto& model_registration : plugin->get_model_registrations())
                {
                    auto model_definition = model_registration->model_definition;
                    const auto& model_name = model_definition.get_model_name();

                    api::IValidator* validator = model_registration->validator.get();
                    validators[model_name] = validator;
                }
            }
            {
                std::vector<api::TriggerPtr> triggers;
                for (auto& t : plugin->get_triggers())
                {
                    triggers.push_back(t);
                };
                std::sort(triggers.begin(), triggers.end(),
                          [](const mindnet::api::TriggerPtr& a, const mindnet::api::TriggerPtr& b)
                          {
                              return a->get_priority() > b->get_priority();
                          });

                for (auto& t : triggers)
                {
                    auto operations = t->get_operations();
                    using plugins::core::enums::Crudl;
                    if (operations.empty())
                        operations = {
                            Crudl::Create,
                            Crudl::Read,
                            Crudl::Update,
                            Crudl::Delete,
                            Crudl::List
                        };
                    for (auto& operation : operations)
                    {
                        trigger_registry_ptr->register_trigger(
                            t->get_table(),
                            t->get_phase(),
                            operation,
                            t
                        );
                    };
                }
            }
        }

        for (auto& e : validators)
        {
            if (!VALIDATION_ENABLED) break;
            e.second->set_validator_func(
                [this](const std::string& name)
                {
                    return this->get_validator(name);
                }
            );
        }
    }

    Service::~Service()
    {
        for (auto& e : validators)
        {
            delete e.second;
        }
    };

    bool Service::has_model(const std::string& model_name)
    {
        return db_ptr->has_model_with_name(model_name);
    }

    std::vector<std::string>& Service::list_model_names()
    {
        return db_ptr->list_model_names();
    };

    std::pair<int, OperationResult> Service::create(const ModelDefinition& def, http::LoginToken& token,
                                                    entity_fields& fields)
    {
        auto result = can_create(def.get_model_name(), token, fields);
        if (result.ko())
        {
            return {-1, result};
        }

        return db_ptr->create(def, token, fields);
    };

    std::pair<entity_fields, OperationResult> Service::read(const ModelDefinition& def, http::LoginToken& token,
                                                            int id)
    {
        debug << "Calling read for " << def.get_model_name() << commit;
        auto result = can_read(def.get_model_name(), token, id);
        if (result.ko())
        {
            return {{}, result};
        }

        trigger_registry_ptr->execute(
            def.get_model_name(),
            TriggerPhase::Before,
            plugins::core::enums::Crudl::Read,
            result,
            {},
            def,
            token.user_id,
            id
        );
        return db_ptr->read(def, token, id);
    };

    OperationResult Service::update(const ModelDefinition& def, http::LoginToken& token, int id,
                                    entity_fields& fields)
    {
        auto result = can_update(def.get_model_name(), token, fields);
        if (result.ko())
        {
            return result;
        }

        return db_ptr->update(def, token, id, fields);
    };

    OperationResult Service::remove(ModelDefinition& def, http::LoginToken& token, int id)
    {
        return db_ptr->remove(def, token, id);
    };

    std::pair<std::vector<entity_fields>, OperationResult> Service::list(
        ModelDefinition& def, http::LoginToken& token, http::QueryParams& query_params)
    {
        return db_ptr->list(def, token, query_params);
    };

    std::optional<ModelDefinition> Service::get_model_definition(const string& model_name)
    {
        return db_ptr->get_model_definition(model_name);
    };

    entity_fields Service::request_to_entity_fields(
        crow::json::rvalue& body, plugins::core::enums::Crudl crudl, ModelDefinition& def
    )
    {
        return db_ptr->request_to_entity_fields(body, crudl, def);
    };

    api::IValidator* Service::get_validator(const std::string& name)
    {
        return validators.count(name) ? validators[name] : nullptr;
    }

    OperationResult Service::can_create(const ModelDefinition& model_definition, http::LoginToken& token,
                                        entity_fields& ef)
    {
        if (!VALIDATION_ENABLED) return ok_result;
        //Authentication
        if (token.ko() && model_definition.get_model_name() != "user")
            return {
                401, "Only logged in users can create."
            };

        api::IValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_create(db_ptr, token, ef);
        }

        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation CREATE cannot be validated."
        };
    };

    OperationResult Service::can_read(const ModelDefinition& model_definition, http::LoginToken& token, int id)
    {
        if (!VALIDATION_ENABLED) return ok_result;
        //Authentication
        if (token.ko() && g_configuration.access_mode == AccessMode::AuthenticatedOnly)
            return {
                401, "Only logged in users can read."
            };

        api::IValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_read(db_ptr, token, id);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation READ cannot be validated."
        };
    }

    OperationResult Service::can_update(const ModelDefinition& model_definition, http::LoginToken& token,
                                        entity_fields& ef)
    {
        if (!VALIDATION_ENABLED) return ok_result;
        //Authentication
        if (token.ko()) return {401, "Only logged in users can update."};

        api::IValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_update(db_ptr, token, ef);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation UPDATE cannot be validated."
        };
    }

    OperationResult Service::can_delete(const ModelDefinition& model_definition, http::LoginToken& token, int id)
    {
        if (!VALIDATION_ENABLED) return ok_result;
        //Authentication
        if (token.ko()) return {401, "Only logged in users can delete."};

        api::IValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_delete(db_ptr, token, id);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation DELETE cannot be validated."
        };
    }

    OperationResult Service::can_list(const ModelDefinition& model_definition, http::LoginToken& token,
                                      string_map& filter)
    {
        if (!VALIDATION_ENABLED) return ok_result;
        //Authentication
        if (token.ko() && g_configuration.access_mode == AccessMode::AuthenticatedOnly)
            return {
                401, "Only logged in users can list."
            };
        api::IValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_list(db_ptr, token, filter);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation LIST cannot be validated."
        };
    }

    const api::PluginRegistryPtr Service::get_plugin_registry() const
    {
        return plugin_registry_ptr;
    }
}
