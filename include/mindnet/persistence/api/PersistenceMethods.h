//
// Created by robertvokac on 9/6/25.
//

#ifndef MIND_NET_PERSISTENCEMETHODS_H
#define MIND_NET_PERSISTENCEMETHODS_H
#include <utility>

#include "../../OperationResult.h"
#include "RequestContext.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
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
#define gen_find_h(plugin, Model, model) \
std::pair<plugins :: plugin :: models::Model, string> find_##model(const RequestContext& ctx, int id);

#define gen_find_cpp(plugin, Model, model, MODEL)\
std::pair< mindnet::plugins:: plugin ::models::Model, string> find_##model(const RequestContext& ctx, int id)\
    {\
        auto result = ctx.db->read(mindnet::plugins:: plugin ::models::MODEL##_DEFINITION, ctx.token, id);\
        if (result.second.ko()) return {{}, result.second.error};\
        mindnet::plugins:: plugin ::models::Model entity;\
        entity.from_values(result.first);\
        return {entity, ""};\
    }

namespace mindnet::persistence::api
{
    std::pair<plugins::core::models::User, mindnet::OperationResult> find_logged_user(
        DbPtr& db, http::LoginToken token);

    bool has_user_name(const RequestContext& ctx, string user_name);
    bool has_user_email(const RequestContext& ctx, string user_mail);
    bool has_map_name(const RequestContext& ctx, string map_name);
    string is_member_of_team(const RequestContext& ctx, int team_id);
    std::pair<int, string> find_note_for_content(const RequestContext& ctx, int content_id);
    bool has_right_for_map(const RequestContext& ctx, const int map_id, const plugins::core::enums::SingleRight single_right);

    gen_find_h(zettelkasten, Collection, collection)
    gen_find_h(zettelkasten, CollectionItem, collection_item)
    gen_find_h(chat, Comment, comment)
    gen_find_h(zettelkasten, Content, content)
    gen_find_h(chat, Discussion, discussion)
    gen_find_h(core, History, history)
    gen_find_h(zettelkasten, Link, link)
    gen_find_h(zettelkasten, Map, map)
    gen_find_h(mail, Message, message)
    gen_find_h(zettelkasten, Note, note)
    gen_find_h(zettelkasten, Property, property)
    gen_find_h(zettelkasten, Question, question)
    gen_find_h(zettelkasten, Reference, reference)
    gen_find_h(test, Review, review)
    gen_find_h(test, SM2State, sm2state)
    gen_find_h(suggestion, Suggestion, suggestion)
    gen_find_h(suggestion, SuggestionReview, suggestion_review)
    gen_find_h(zettelkasten, Tag, tag)
    gen_find_h(zettelkasten, TagType, tag_type)
    gen_find_h(core, Team, team)
    gen_find_h(core, TeamMember, team_member)
    gen_find_h(core, User, user)
}


#endif //MIND_NET_PERSISTENCEMETHODS_H