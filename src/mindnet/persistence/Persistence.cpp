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
#include "mindnet/persistence/api/MapCrudlValidator.h"

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

        api::CrudlValidator* map_validator = new api::MapCrudlValidator();
        validators["map"] = map_validator;
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
        api::CrudlValidator* v = get_validator("*");
        if (v != nullptr)
        {
            string result = v->can_create(this, ef);
            if (!result.empty())
            {
                return result;
            }
        }

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
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation cannot be validated.";
        }


    return "";
};

    string Persistence::can_read(const ModelDefinition& model_definition, int id)
    {
        api::CrudlValidator* v = get_validator("*");
        if (v != nullptr)
        {
            string result = v->can_read(this, id);
            if (!result.empty())
            {
                return result;
            }
        }

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
            return "Validator is not implemented for " + model_definition.get_model_name() + ". Operation cannot be validated.";
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

bool Persistence::update(int id, entity_fields& fields, models::misc::ModelDefinition& def, string& error)
{
    return get_repository(def.get_model_name())->update(id, fields, error);
}

bool Persistence::remove(int id, models::misc::ModelDefinition& def, string& error)
{
    return get_repository(def.get_model_name())->remove(id, error);
}

std::optional<ModelDefinition> Persistence::get_model_definition(string& model_name)
{
    if (!has_repository(model_name))
    {
        return std::nullopt;
    }
    return get_repository(model_name)->get_model_definition();
}

std::vector<entity_fields> Persistence::list(http::QueryParams& query_params, ModelDefinition& def, string& error)
{
    // auto result = get_repository(def.get_model_name())->list(query_params, error);
    // std::vector<entity_fields> duplicated;
    // for (int i = 0; i < 100; i++)
    // {
    //     duplicated.insert(duplicated.end(), result.begin(), result.end());
    // }
    // return duplicated;
    return get_repository(def.get_model_name())->list(query_params, error);
}

entity_fields Persistence::request_to_entity_fields(
    crow::json::rvalue& body, const enums::Crudl crudl, models::misc::ModelDefinition& def)
{
    return get_repository(def.get_model_name())->request_to_entity_fields(body, crudl);
}

}
