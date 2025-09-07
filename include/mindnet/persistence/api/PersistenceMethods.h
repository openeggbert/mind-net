//
// Created by robertvokac on 9/6/25.
//

#ifndef MIND_NET_PERSISTENCEMETHODS_H
#define MIND_NET_PERSISTENCEMETHODS_H
#include <utility>

#include "CrudlValidatorBase.h"
#include "IPersistence.h"
#include "OperationResult.h"
#include "mindnet/http/LoginToken.h"
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
#define gen_find_h(Model, model) \
std::pair<models::Model, string> find_##model(db_ptr& db, http::LoginToken& token, int id);

#define gen_find_cpp(Model, model, MODEL)\
std::pair<models::Model, string> find_##model(db_ptr& db, http::LoginToken& token, int id)\
    {\
        auto result = db->read(id, models::MODEL##_DEFINITION, token);\
        if (result.second.ko()) return {{}, result.second.error};\
        models::Model entity;\
        entity.from_values(result.first);\
        return {entity, ""};\
    }

namespace mindnet::persistence::api
{
    using db_ptr = mindnet::persistence::IPersistence*;

    std::pair<models::User, api::OperationResult> find_logged_user(
        db_ptr& db, http::LoginToken token);

    bool has_user_name(const ValidatorContext& ctx, string user_name);
    bool has_user_email(const ValidatorContext& ctx, string user_mail);
    bool has_map_name(const ValidatorContext& ctx, string map_name);

    string is_member_of_team(const ValidatorContext& ctx, int team_id);

    gen_find_h(Collection, collection)
    gen_find_h(CollectionItem, collection_item)
    gen_find_h(Comment, comment)
    gen_find_h(Content, content)
    gen_find_h(Discussion, discussion)
    gen_find_h(History, history)
    gen_find_h(Link, link)
    gen_find_h(Map, map)
    gen_find_h(Message, message)
    gen_find_h(Note, note)
    gen_find_h(Property, property)
    gen_find_h(Question, question)
    gen_find_h(Reference, reference)
    gen_find_h(Review, review)
    gen_find_h(SM2State, sm2state)
    gen_find_h(Suggestion, suggestion)
    gen_find_h(SuggestionReview, suggestion_review)
    gen_find_h(Tag, tag)
    gen_find_h(TagType, tag_type)
    gen_find_h(Team, team)
    gen_find_h(TeamMember, team_member)
    gen_find_h(User, user)
}


#endif //MIND_NET_PERSISTENCEMETHODS_H