//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/api/Persistence.h"

#include "mindnet/Configuration.h"
#include "mindnet/Global.h"
#include "mindnet/http/LoginToken.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/plugins/mail/models/Message.h"
#include "mindnet/plugins/core/models/Team.h"
#include "mindnet/plugins/core/models/TeamMember.h"
#include "mindnet/plugins/chat/models/Discussion.h"
#include "mindnet/plugins/chat/models/Comment.h"
#include "mindnet/plugins/suggestion/models/Suggestion.h"
#include "mindnet/plugins/suggestion/models/SuggestionReview.h"
#include "mindnet/plugins/core/models/History.h"
#include "mindnet/plugins/zettelkasten/models/Map.h"
#include "mindnet/plugins/zettelkasten/models/Content.h"
#include "mindnet/plugins/zettelkasten/models/Note.h"
#include "mindnet/plugins/zettelkasten/models/Property.h"
#include "mindnet/plugins/zettelkasten/models/TagType.h"
#include "mindnet/plugins/zettelkasten/models/Tag.h"
#include "mindnet/plugins/zettelkasten/models/Collection.h"
#include "mindnet/plugins/zettelkasten/models/CollectionItem.h"
#include "mindnet/plugins/test/models/Review.h"
#include "mindnet/plugins/test/models/SM2State.h"
#include "mindnet/plugins/zettelkasten/models/Question.h"
#include "mindnet/plugins/zettelkasten/models/Reference.h"
#include "mindnet/plugins/zettelkasten/models/Link.h"

#include "../../../../include/mindnet/persistence/impl/sqlite/Convertors.h"
#include "../../../../include/mindnet/persistence/impl/sqlite/RepositoryImplSqlite.h"

#define MODEL_JOIN(x) x##_DEFINITION
#define FUNCTION_JOIN(x) request_to_entity_fields_##x

#define add_repository(plugin, model, Model, MODEL) \
api::IRepository* model##_repo = new RepositoryImplSqlite(\
& FUNCTION_JOIN(model),\
plugins:: plugin :: models :: MODEL_JOIN(MODEL)\
);\
repositories[#model] = model##_repo;\
repository_names.emplace_back(#model);

namespace mindnet::persistence::api
{
    using namespace mindnet::persistence::impl::sqlite;

    Persistence::Persistence()
    {
        // IRepository* user_repo =
        //     new RepositoryImplSqlite(
        //         &request_to_entity_fields_user,
        //         models::USER_DEFINITION
        //         );
        // repositories["user"] = user_repo;;

        add_repository(core, user, User, USER);
        add_repository(core, team, Team, TEAM);
        add_repository(core, team_member, TeamMember, TEAM_MEMBER);
        add_repository(core, history, History, HISTORY);
        //
        add_repository(zettelkasten, map, Map, MAP);
        add_repository(zettelkasten, content, Content, CONTENT);
        add_repository(zettelkasten, note, Note, NOTE);
        add_repository(zettelkasten, property, Property, PROPERTY);
        add_repository(zettelkasten, tag_type, TagType, TAG_TYPE);
        add_repository(zettelkasten, tag, Tag, TAG);
        add_repository(zettelkasten, collection, Collection, COLLECTION);
        add_repository(zettelkasten, collection_item, CollectionItem, COLLECTION_ITEM);
        add_repository(zettelkasten, question, Question, QUESTION);
        add_repository(zettelkasten, reference, Reference, REFERENCE);
        add_repository(zettelkasten, link, Link, LINK);
        //
        add_repository(test, review, Review, REVIEW);
        add_repository(test, sm2_state, SM2State, SM2_STATE);
        //
        add_repository(chat, discussion, Discussion, DISCUSSION);
        add_repository(chat, comment, Comment, COMMENT);
        //
        add_repository(mail, message, Message, MESSAGE);
        //
        add_repository(suggestion,  suggestion, Suggestion, SUGGESTION);
        add_repository(suggestion,suggestion_review, SuggestionReview, SUGGESTION_REVIEW);
        //


    }

    Persistence::~Persistence()
    {
        for (auto& e : Persistence::list_model_names())
        {
            delete get_repository(e);
        }

    }

    api::IRepository* Persistence::get_repository(const std::string& name)
    {
        return repositories.count(name) ? repositories[name] : nullptr;
    }



    bool Persistence::has_model_with_name(const std::string& name)
    {
        return repositories.count(name) > 0;
    }

    std::vector<std::string>& Persistence::list_model_names()
    {
        return repository_names;
    }



    std::pair<int, OperationResult> Persistence::create(
        const model::ModelDefinition& def,
        http::LoginToken& token,
        entity_fields& fields)
    {

        string error;
        int last_id = get_repository(def.get_model_name())->create(fields, error);
        return {last_id, {500, error}};
    }

    std::pair<entity_fields, OperationResult> Persistence::read(const model::ModelDefinition& def,
                                    http::LoginToken& token, const int id)
    {

        string error;
        entity_fields ef = get_repository(def.get_model_name())->read(id, error);
        if (error.empty())
        {
            return {ef, {}};
        } else
        {
            return {{}, {500, error}};
        }

    }

    OperationResult Persistence::update(
        const model::ModelDefinition& def, http::LoginToken& token,
        int id, entity_fields& fields
                             )
    {

        string error;
        get_repository(def.get_model_name())->update(id, fields, error);
        if (error.empty()) {return ok_result;} else {return {500, error};}
    }

    OperationResult Persistence::remove(model::ModelDefinition& def, http::LoginToken& token, int id)
    {
        string_map empty_map;

        string error;
        get_repository(def.get_model_name())->remove(id, error);
        if (error.empty()) {return ok_result;} else {return {500, error};}

    }

    std::pair<std::vector<entity_fields>, OperationResult> Persistence::list(
                                                 ModelDefinition& def,
                                                 http::LoginToken& token,
                                                 http::QueryParams& query_params)
    {

        string error;
        auto l = get_repository(def.get_model_name())->list(query_params, error);
        if (error.empty())
        {
            return {l, ok_result};
        } else
        {
            return {{}, {500, error}};
        }

    }

    std::optional<ModelDefinition> Persistence::get_model_definition(const string& model_name)
    {
        if (!has_model_with_name(model_name))
        {
            warn << "Model " << model_name << " not found" << commit;
            return std::nullopt;
        }
        return get_repository(model_name)->get_model_definition();
    }

    entity_fields Persistence::request_to_entity_fields(
        crow::json::rvalue& body, const plugins::core::enums::Crudl crudl, model::ModelDefinition& def)
    {
        return get_repository(def.get_model_name())->request_to_entity_fields(body, crudl);
    }

}
