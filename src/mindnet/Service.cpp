//
// Created by robertvokac on 9/13/25.
//

#include "mindnet/Service.h"
#include "mindnet/plugins/core/validators/UserValidator.h"
#include "mindnet/plugins/mail/validators/MessageValidator.h"
#include "mindnet/plugins/core/validators/TeamValidator.h"
#include "mindnet/plugins/core/validators/TeamMemberValidator.h"
#include "mindnet/plugins/chat/validators/DiscussionValidator.h"
#include "mindnet/plugins/chat/validators/CommentValidator.h"
#include "mindnet/plugins/suggestion/validators/SuggestionValidator.h"
#include "mindnet/plugins/suggestion/validators/SuggestionReviewValidator.h"
#include "mindnet/plugins/core/validators/HistoryValidator.h"
#include "mindnet/plugins/zettelkasten/validators/MapValidator.h"
#include "mindnet/plugins/zettelkasten/validators/ContentValidator.h"
#include "mindnet/plugins/zettelkasten/validators/NoteValidator.h"
#include "mindnet/plugins/zettelkasten/validators/PropertyValidator.h"
#include "mindnet/plugins/zettelkasten/validators/TagTypeValidator.h"
#include "mindnet/plugins/zettelkasten/validators/TagValidator.h"
#include "mindnet/plugins/zettelkasten/validators/CollectionValidator.h"
#include "mindnet/plugins/zettelkasten/validators/CollectionItemValidator.h"
#include "mindnet/plugins/test/validators/ReviewValidator.h"
#include "mindnet/plugins/test/validators/SM2StateValidator.h"
#include "mindnet/plugins/zettelkasten/validators/QuestionValidator.h"
#include "mindnet/plugins/zettelkasten/validators/ReferenceValidator.h"
#include "mindnet/plugins/zettelkasten/validators/LinkValidator.h"
//
#define add_validator(plugin, model, Model)\
api::IValidator* model##_validator = new mindnet::plugins:: plugin ::validators:: Model##Validator();\
validators[#model] = model##_validator;

namespace mindnet
{
    using validator = api::IValidator*;
    using mindnet::OperationResult;
    using model::ModelDefinition;

    Service::Service(const DbPtr& db_, const api::PluginRegistryPtr& plugin_registry_ptr_) : IService(db_), db_ptr(db_), plugin_registry_ptr(plugin_registry_ptr_)
    {
        //#define disable_validation

#ifndef disable_validation

        add_validator(core, user, User)
        add_validator(core, team, Team)
        add_validator(core, team_member, TeamMember)
        add_validator(core, history, History)
        //
        add_validator(mail, message, Message)
        //
        add_validator(chat, discussion, Discussion)
        add_validator(chat, comment, Comment)
        //
        add_validator(suggestion, suggestion, Suggestion)
        add_validator(suggestion, suggestion_review, SuggestionReview)
        //
        add_validator(zettelkasten, map, Map)
        add_validator(zettelkasten, content, Content)
        add_validator(zettelkasten, note, Note)
        add_validator(zettelkasten, property, Property)
        add_validator(zettelkasten, tag_type, TagType)
        add_validator(zettelkasten, tag, Tag)
        add_validator(zettelkasten, collection, Collection)
        add_validator(zettelkasten, collection_item, CollectionItem)
        add_validator(zettelkasten, question, Question)
        add_validator(zettelkasten, reference, Reference)
        add_validator(zettelkasten, link, Link)
        //
        add_validator(test, review, Review)
        add_validator(test, sm2_state, SM2State)

        for (auto& e : validators)
        {
            e.second->set_validator_func(
                [this](const std::string& name)
                {
                    return this->get_validator(name);
                }
            );
        }

#endif
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
#ifndef disable_validation

        auto result = can_create(def.get_model_name(), token, fields);
        if (result.ko())
        {
            return {-1, result};
        }
#endif
        return db_ptr->create(def, token, fields);
    };

    std::pair<entity_fields, OperationResult> Service::read(const ModelDefinition& def, http::LoginToken& token, int id)
    {
#ifndef disable_validation

        auto result = can_read(def.get_model_name(), token, id);
        if (result.ko())
        {
            return {{}, result};
        }
#endif
        return db_ptr->read(def, token, id);
    };

    OperationResult Service::update(const ModelDefinition& def, http::LoginToken& token, int id, entity_fields& fields)
    {
#ifndef disable_validation

        auto result = can_update(def.get_model_name(), token, fields);
        if (result.ko())
        {
            return result;
        }
#endif
        return db_ptr->update(def, token, id, fields);
    };

    OperationResult Service::remove(ModelDefinition& def, http::LoginToken& token, int id)
    {
#ifndef disable_validation

        auto result = can_delete(def.get_model_name(), token, id);
        if (result.ko())
        {
            return result;
        }
#endif
        return db_ptr->remove(def, token, id);
    };

    std::pair<std::vector<entity_fields>, OperationResult> Service::list(
        ModelDefinition& def, http::LoginToken& token, http::QueryParams& query_params)
    {
#ifndef disable_validation

        auto result = can_list(def.get_model_name(), token, query_params.filters);
        if (result.ko())
        {
            return {{}, result};
        }
#endif
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
        //Authentication
        if (token.ko() && model_definition.get_model_name() != "user") return {401, "Only logged in users can create."};

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
        //Authentication
        if (token.ko() && !g_configuration.allow_public_access) return {401, "Only logged in users can read."};

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
        //Authentication
        if (token.ko() && !g_configuration.allow_public_access) return {401, "Only logged in users can list."};
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
