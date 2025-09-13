//
// Created by robertvokac on 9/13/25.
//

#include "mindnet/Service.h"
#include "mindnet/persistence/impl/sqlite/validators/UserCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/MessageCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/TeamCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/TeamMemberCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/DiscussionCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/CommentCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/SuggestionCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/SuggestionReviewCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/HistoryCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/MapCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/ContentCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/NoteCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/PropertyCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/TagTypeCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/TagCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/CollectionCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/CollectionItemCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/ReviewCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/SM2StateCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/QuestionCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/ReferenceCrudlValidator.h"
#include "mindnet/persistence/impl/sqlite/validators/LinkCrudlValidator.h"
//
#define add_validator(model, Model)\
persistence::api::ICrudlValidator* model##_validator = new mindnet::persistence::impl::sqlite::validators:: Model##CrudlValidator();\
validators[#model] = model##_validator;

namespace mindnet
{
    using validator = persistence::api::ICrudlValidator*;
    using persistence::api::OperationResult;
    using models::misc::ModelDefinition;


    Service::Service(const DbPtr& db_) : IService(db_), db_ptr(db_)
    {
        //#define disable_validation

#ifndef disable_validation

        add_validator(user, User)
        add_validator(message, Message)
        add_validator(team, Team)
        add_validator(team_member, TeamMember)
        add_validator(discussion, Discussion)
        add_validator(comment, Comment)
        add_validator(suggestion, Suggestion)
        add_validator(suggestion_review, SuggestionReview)
        add_validator(history, History)
        add_validator(map, Map)
        add_validator(content, Content)
        add_validator(note, Note)
        add_validator(property, Property)
        add_validator(tag_type, TagType)
        add_validator(tag, Tag)
        add_validator(collection, Collection)
        add_validator(collection_item, CollectionItem)
        add_validator(review, Review)
        add_validator(sm2_state, SM2State)
        add_validator(question, Question)
        add_validator(reference, Reference)
        add_validator(link, Link)

        for (auto& e : validators) {
            e.second->set_validator_func(
                [this](const std::string& name) {
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
        crow::json::rvalue& body, enums::Crudl crudl, ModelDefinition& def
    )
    {
        return db_ptr->request_to_entity_fields(body, crudl, def);
    };

    persistence::api::ICrudlValidator* Service::get_validator(const std::string& name)
    {
        return validators.count(name) ? validators[name] : nullptr;
    }


    OperationResult Service::can_create(const ModelDefinition& model_definition, http::LoginToken& token,
                                        entity_fields& ef)
    {
        //Authentication
        if (token.ko() && model_definition.get_model_name() != "user") return {401, "Only logged in users can create."};

        persistence::api::ICrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_create(db_ptr, token, ef);
        }

        return {500, "Validator is not implemented for " + model_definition.get_model_name() +
                               ". Operation CREATE cannot be validated."};
    };

    OperationResult Service::can_read(const ModelDefinition& model_definition, http::LoginToken& token, int id)
    {
        //Authentication
        if (token.ko() && !g_configuration.allow_public_access) return {401, "Only logged in users can read."};


        persistence::api::ICrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_read(db_ptr, token, id);
        }
        return {500, "Validator is not implemented for " + model_definition.get_model_name() +
                               ". Operation READ cannot be validated."};
    }

    OperationResult Service::can_update(const ModelDefinition& model_definition, http::LoginToken& token,
                                        entity_fields& ef)
    {
        //Authentication
        if (token.ko()) return {401, "Only logged in users can update."};

        persistence::api::ICrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_update(db_ptr, token, ef);
        }
        return {500, "Validator is not implemented for " + model_definition.get_model_name() +
                               ". Operation UPDATE cannot be validated."};

    }

    OperationResult Service::can_delete(const ModelDefinition& model_definition, http::LoginToken& token, int id)
    {
        //Authentication
        if (token.ko()) return {401, "Only logged in users can delete."};

        persistence::api::ICrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_delete(db_ptr, token, id);
        }
        return {500, "Validator is not implemented for " + model_definition.get_model_name() +
                               ". Operation DELETE cannot be validated."};
    }

    OperationResult Service::can_list(const ModelDefinition& model_definition, http::LoginToken& token,
                                      string_map& filter)
    {
        //Authentication
        if (token.ko() && !g_configuration.allow_public_access) return {401, "Only logged in users can list."};
        persistence::api::ICrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            return v2->can_list(db_ptr, token, filter);
        }
        return {500, "Validator is not implemented for " + model_definition.get_model_name() +
                               ". Operation LIST cannot be validated."};

    }
}
