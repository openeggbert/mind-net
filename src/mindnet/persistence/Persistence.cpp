//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/Persistence.h"

#include "mindnet/Global.h"
#include "mindnet/models/User.h"
#include "mindnet/models/Message.h"
#include "mindnet/models/Team.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/models/Discussion.h"
#include "mindnet/models/Comment.h"
#include "mindnet/models/Suggestion.h"
#include "mindnet/models/SuggestionReview.h"
#include "mindnet/models/History.h"
#include "mindnet/models/Map.h"
#include "mindnet/models/Content.h"
#include "mindnet/models/Note.h"
#include "mindnet/models/Property.h"
#include "mindnet/models/TagType.h"
#include "mindnet/models/Tag.h"
#include "mindnet/models/Collection.h"
#include "mindnet/models/CollectionItem.h"
#include "mindnet/models/Review.h"
#include "mindnet/models/SM2State.h"
#include "mindnet/models/Question.h"
#include "mindnet/models/Reference.h"
#include "mindnet/models/Link.h"
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

#include "mindnet/persistence/impl/sqlite/repositories/Convertors.h"
#include "mindnet/persistence/impl/sqlite/repositories/RepositoryImplSqlite.h"

#define MODEL_JOIN(x) x##_DEFINITION
#define FUNCTION_JOIN(x) request_to_entity_fields_##x

#define add_repository(model, Model, MODEL) \
api::IRepository* model##_repo = new RepositoryImplSqlite(\
& FUNCTION_JOIN(model),\
models :: MODEL_JOIN(MODEL)\
);\
repositories[#model] = model##_repo;\
repository_names.emplace_back(#model);

#define add_validator(model, Model)\
api::CrudlValidator* model##_validator = new mindnet::persistence::impl::sqlite::validators:: Model##CrudlValidator();\
validators[#model] = model##_validator;

namespace mindnet::persistence
{
    using namespace mindnet::persistence::impl::sqlite::repositories;
    using db_ = std::shared_ptr<mindnet::persistence::Persistence>;

    Persistence::Persistence()
    {
        // IRepository* user_repo =
        //     new RepositoryImplSqlite(
        //         &request_to_entity_fields_user,
        //         models::USER_DEFINITION
        //         );
        // repositories["user"] = user_repo;;

        add_repository(user, User, USER);
        add_repository(message, Message, MESSAGE);
        add_repository(team, Team, TEAM);
        add_repository(team_member, TeamMember, TEAM_MEMBER);
        add_repository(discussion, Discussion, DISCUSSION);
        add_repository(comment, Comment, COMMENT);
        add_repository(suggestion, Suggestion, SUGGESTION);
        add_repository(suggestion_review, SuggestionReview, SUGGESTION_REVIEW);
        add_repository(history, History, HISTORY);
        add_repository(map, Map, MAP);
        add_repository(content, Content, CONTENT);
        add_repository(note, Note, NOTE);
        add_repository(property, Property, PROPERTY);
        add_repository(tag_type, TagType, TAG_TYPE);
        add_repository(tag, Tag, TAG);
        add_repository(collection, Collection, COLLECTION);
        add_repository(collection_item, CollectionItem, COLLECTION_ITEM);
        add_repository(review, Review, REVIEW);
        add_repository(sm2_state, SM2State, SM2_STATE);
        add_repository(question, Question, QUESTION);
        add_repository(reference, Reference, REFERENCE);
        add_repository(link, Link, LINK);
//
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
    }

    Persistence::~Persistence()
    {
        for (auto& e : list_repositories())
        {
            delete get_repository(e);
        }
    }

    api::IRepository* Persistence::get_repository(const std::string& name)
    {
        return repositories.count(name) ? repositories[name] : nullptr;
    }

    api::CrudlValidator* Persistence::get_validator(const std::string& name)
    {
        return validators.count(name) ? validators[name] : nullptr;
    }


    bool Persistence::has_repository(const std::string& name)
    {
        return repositories.count(name) > 0;
    }

    std::vector<std::string>& Persistence::list_repositories()
    {
        return repository_names;
    }

    string Persistence::can_create(const ModelDefinition& model_definition, entity_fields& ef)
    {
        api::CrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            string result = v2->can_create(this, ef);
            if (!result.empty())
            {
                return result;
            }
        } else
        {
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation CREATE cannot be validated.";
        }


    return "";
};

    string Persistence::can_read(const ModelDefinition& model_definition, int id)
    {
        api::CrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            string result = v2->can_read(this, id);
            if (!result.empty())
            {
                return result;
            }
        } else
        {
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation READ cannot be validated.";
        }


        return "";
    }

    string Persistence::can_update(const ModelDefinition& model_definition, entity_fields& ef)
    {
        api::CrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            string result = v2->can_update(this, ef);
            if (!result.empty())
            {
                return result;
            }
        } else
        {
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation UPDATE cannot be validated.";
        }


        return "";
    }

    string Persistence::can_delete(const ModelDefinition& model_definition, int id)
    {
        api::CrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            string result = v2->can_delete(this, id);
            if (!result.empty())
            {
                return result;
            }
        } else
        {
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation DELETE cannot be validated.";
        }


        return "";
    }

    string Persistence::can_list(const ModelDefinition& model_definition,
        std::map<std::string, std::string>& filter)
    {
        api::CrudlValidator* v2 = get_validator(model_definition.get_model_name());
        if (v2 != nullptr)
        {
            string result = v2->can_list(this, filter);
            if (!result.empty())
            {
                return result;
            }
        } else
        {
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation LIST cannot be validated.";
        }


        return "";
    };


int Persistence::create(const models::misc::ModelDefinition& def, entity_fields& fields, string& error)
{
    std::map<std::string, std::string> empty_map;
    auto result = can_create(def.get_model_name(), fields);
    if (!result.empty())
    {
        error = result;
        return -1;
    }
    return get_repository(def.get_model_name())->create(fields, error);
}

entity_fields Persistence::read(const int id, const models::misc::ModelDefinition& def, string& error)
{
    std::map<std::string, std::string> empty_map;
    auto result = can_read(def.get_model_name(), id);
    if (!result.empty())
    {
        error = result;
        return {};
    }
    return get_repository(def.get_model_name())->read(id, error);
}

bool Persistence::update(int id, entity_fields& fields, const models::misc::ModelDefinition& def, string& error)
{
    std::map<std::string, std::string> empty_map;
    auto result = can_update(def.get_model_name(), fields);
    if (!result.empty())
    {
        error = result;
        return -1;
    }
    return get_repository(def.get_model_name())->update(id, fields, error);
}

bool Persistence::remove(int id, models::misc::ModelDefinition& def, string& error)
{
    std::map<std::string, std::string> empty_map;
    auto result = can_delete(def.get_model_name(), id);
    if (!result.empty())
    {
        error = result;
        return -1;
    }
    return get_repository(def.get_model_name())->remove(id, error);
}

std::optional<ModelDefinition> Persistence::get_model_definition(const string& model_name)
{
    if (!has_repository(model_name))
    {
        return std::nullopt;
    }
    return get_repository(model_name)->get_model_definition();
}

std::vector<entity_fields> Persistence::list(http::QueryParams& query_params, ModelDefinition& def, string& error)
{
    std::map<std::string, std::string> empty_map;
    auto result = can_list(def.get_model_name(), query_params.filters);
    if (!result.empty())
    {
        error = result;
        return {};
    }
    return get_repository(def.get_model_name())->list(query_params, error);
}

entity_fields Persistence::request_to_entity_fields(
    crow::json::rvalue& body, const enums::Crudl crudl, models::misc::ModelDefinition& def)
{
    return get_repository(def.get_model_name())->request_to_entity_fields(body, crudl);
}

}
