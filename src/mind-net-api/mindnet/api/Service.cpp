//
// Created by robertvokac on 9/13/25.
//

#include "mindnet/api/Service.h"
#include "mindnet/essential/Global.h"
//

namespace mindnet::api
{
    using_loggers()

    static constexpr bool VALIDATION_ENABLED = true;
    static constexpr bool TRIGGERS_ENABLED = true;

    using validator = IValidator*;
    using mindnet::api::OperationResult;
    using model::ModelDefinition;
    using mindnet::essential::Crudl;
    using api::TriggerPhase;
    using api::empty_result;

    Service::Service(
        const api::DbPtr& db_,
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

                    validators[model_name] = model_registration->validator;
                }
            }
            if (TRIGGERS_ENABLED) {
                std::vector<api::TriggerPtr> triggers;
                for (auto& t : plugin->get_triggers())
                {
                    if (t->get_phase() == TriggerPhase::Around)
                    {
                        throw std::runtime_error(
                            std::string("TriggerPhase Around is not yet supported: ") + t->get_name());
                    }
                    if (t->get_phase() == TriggerPhase::InsteadOf)
                    {
                        throw std::runtime_error(
                            std::string("TriggerPhase InsteadOf is not yet supported: ") + t->get_name());
                    }
                    triggers.push_back(t);
                };
                std::sort(triggers.begin(), triggers.end(),
                          [](const mindnet::api::TriggerPtr& a, const mindnet::api::TriggerPtr& b)
                          {
                              return a->get_priority() > b->get_priority();
                          });

                for (auto& trigger : triggers)
                {
                    {
                        trigger->set_service_ptr(this);
                        trigger->set_create_fn(&Service::create);
                        trigger->set_read_fn(&Service::read);
                        trigger->set_update_fn(&Service::update);
                        trigger->set_delete_fn(&Service::remove);
                        trigger->set_list_fn(&Service::list);
                    }
                    auto operations = trigger->get_operations();
                    using mindnet::essential::Crudl;
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
                            trigger->get_table(),
                            trigger->get_phase(),
                            operation,
                            trigger
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
    };

    bool Service::has_model(const std::string& model_name)
    {
        return db_ptr->has_model_with_name(model_name);
    }

    std::vector<std::string>& Service::list_model_names()
    {
        return db_ptr->list_model_names();
    }

    static constexpr int MAX_TRIGGER_DEPTH = 32;

    std::pair<int, OperationResult> Service::create(const ModelDefinition& def, api::LoginToken& token,
                                                    entity_fields& fields, int stack_depth)
    {
        if(stack_depth > MAX_TRIGGER_DEPTH) return {-1, {500, "Max trigger depth exceeded"}};

        auto action = Crudl::Create;
        auto validation_result = can_create(def.get_model_name(), token, fields);
        trigger_registry_ptr->execute(TriggerPhase::Before, action, stack_depth, validation_result, empty_result, def, token.user_id, 0, fields);
        if (validation_result.ko())
        {
            return {-1, validation_result};
        }
        auto action_result = db_ptr->create(def, token, fields);
        trigger_registry_ptr->execute(TriggerPhase::After, action, stack_depth, validation_result, action_result.second, def, token.user_id, action_result.first, fields);
        if (validation_result.ko())
        {
            return {-1, validation_result};
        }
        return action_result;
    };

    std::pair<entity_fields, OperationResult> Service::read(const ModelDefinition& def, api::LoginToken& token,
                                                            int id, int stack_depth)
    {
        if(stack_depth > MAX_TRIGGER_DEPTH) return {api::empty_entity_fields, {500, "Max trigger depth exceeded"}};

        if (id == 0)
        {
            return {{}, {400, "You cannot read using id=0"}};
        }
        auto action = Crudl::Read;
        debug << "Calling read for " << def.get_model_name() << commit;
        auto validation_result = can_read(def.get_model_name(), token, id);

        trigger_registry_ptr->execute(TriggerPhase::Before, action, stack_depth, validation_result, empty_result, def, token.user_id, id);
        if (validation_result.ko())
        {
            return {{}, validation_result};
        }
        auto action_result = db_ptr->read(def, token, id);
        trigger_registry_ptr->execute(TriggerPhase::After, action, stack_depth, validation_result, action_result.second, def, token.user_id, id, action_result.first);
        if (validation_result.ko())
        {
            return {{}, validation_result};
        }
        return action_result;
    };

    OperationResult Service::update(const ModelDefinition& def, api::LoginToken& token, int id,
                                    entity_fields& fields, int stack_depth)
    {
        if(stack_depth > MAX_TRIGGER_DEPTH) return {500, "Max trigger depth exceeded"};
        auto action = Crudl::Update;
        auto validation_result = can_update(def.get_model_name(), token, fields);
        trigger_registry_ptr->execute(TriggerPhase::Before, action, stack_depth, validation_result, empty_result, def, token.user_id, id, fields);
        if (validation_result.ko())
        {
            return validation_result;
        }
        auto action_result = db_ptr->update(def, token, id, fields);
        trigger_registry_ptr->execute(TriggerPhase::After, action, stack_depth, validation_result, action_result, def, token.user_id, id, fields);
        if (validation_result.ko())
        {
            return validation_result;
        }
        return action_result;
    };

    OperationResult Service::remove(ModelDefinition& def, api::LoginToken& token, int id, int stack_depth)
    {
        if(stack_depth > MAX_TRIGGER_DEPTH) return {500, "Max trigger depth exceeded"};
        auto action = Crudl::Delete;
        auto validation_result = can_delete(def.get_model_name(), token, id);
        trigger_registry_ptr->execute(TriggerPhase::Before, action, stack_depth, validation_result, empty_result, def, token.user_id, id);

        if (validation_result.ko())
        {
            return validation_result;
        }
        auto action_result = db_ptr->remove(def, token, id);
        trigger_registry_ptr->execute(TriggerPhase::After, action, stack_depth, validation_result, action_result, def, token.user_id,
                                      id);
        if (validation_result.ko())
        {
            return validation_result;
        }
        return action_result;
    };

    std::pair<std::vector<entity_fields>, OperationResult> Service::list(
        ModelDefinition& def, api::LoginToken& token, orm::QueryParams& query_params, int stack_depth)
    {
        if(stack_depth > MAX_TRIGGER_DEPTH) return {{}, {500, "Max trigger depth exceeded"}};
        auto action = Crudl::List;
        auto validation_result = can_list(def.get_model_name(), token, query_params.filters);
        trigger_registry_ptr->execute(TriggerPhase::Before, action, stack_depth, validation_result, empty_result, def, token.user_id, 0, api::empty_entity_fields, query_params);

        if (validation_result.ko())
        {
            return {{}, validation_result};
        }
        auto action_result = db_ptr->list(def, token, query_params);
        trigger_registry_ptr->execute(TriggerPhase::After, action, stack_depth, validation_result, action_result.second, def, token.user_id, 0, api::empty_entity_fields, query_params);
        if (validation_result.ko())
        {
            return {{}, validation_result};
        }
        return action_result;
    };

    std::optional<ModelDefinition> Service::get_model_definition(const string& model_name)
    {
        return db_ptr->get_model_definition(model_name);
    };

    entity_fields Service::request_to_entity_fields(
        crow::json::rvalue& body, mindnet::essential::Crudl crudl, ModelDefinition& def
    )
    {
        return db_ptr->request_to_entity_fields(body, crudl, def);
    };

    std::shared_ptr<IValidator> Service::get_validator(const std::string& name)
    {
        return validators.count(name) ? validators[name] : nullptr;
    }

    OperationResult Service::can_create(const ModelDefinition& model_definition, api::LoginToken& token,
                                        entity_fields& ef)
    {
        if (!VALIDATION_ENABLED) return ok_result;

        std::shared_ptr<IValidator> v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_create(db_ptr, token, ef);
        }

        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation CREATE cannot be validated."
        };
    };

    OperationResult Service::can_read(const ModelDefinition& model_definition, api::LoginToken& token, int id)
    {
        if (!VALIDATION_ENABLED) return ok_result;

        std::shared_ptr<IValidator> v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_read(db_ptr, token, id);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation READ cannot be validated."
        };
    }

    OperationResult Service::can_update(const ModelDefinition& model_definition, api::LoginToken& token,
                                        entity_fields& ef)
    {
        if (!VALIDATION_ENABLED) return ok_result;

        std::shared_ptr<IValidator> v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_update(db_ptr, token, ef);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation UPDATE cannot be validated."
        };
    }

    OperationResult Service::can_delete(const ModelDefinition& model_definition, api::LoginToken& token, int id)
    {
        if (!VALIDATION_ENABLED) return ok_result;

        std::shared_ptr<IValidator> v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_delete(db_ptr, token, id);
        }
        return {
            500, "Validator is not implemented for " + model_definition.get_model_name() +
            ". Operation DELETE cannot be validated."
        };
    }

    OperationResult Service::can_list(const ModelDefinition& model_definition, api::LoginToken& token,
                                      string_map& filter)
    {
        if (!VALIDATION_ENABLED) return ok_result;

        std::shared_ptr<IValidator> v2 = get_validator(model_definition.get_model_name());
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
