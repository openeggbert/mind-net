//
// Created by robertvokac on 9/6/25.
//
#ifndef COREPERSISTENCEMETHODS_H
#define COREPERSISTENCEMETHODS_H

#include <utility>

#include "../../api/PersistenceMethods.h"
#include "mindnet/api/IPersistence.h"
#include "mindnet/api/OperationResult.h"
#include "mindnet/api/RequestContext.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/api/LoginToken.h"
#include "mindnet/api/PersistenceMethods.h"
#include "mindnet/plugins/core/models/User.h"
// #include "mindnet/plugins/mail/models/Message.h"
#include "mindnet/plugins/core/models/Team.h"
#include "mindnet/plugins/core/models/TeamMember.h"
// #include "mindnet/plugins/chat/models/Discussion.h"
// #include "mindnet/plugins/chat/models/Comment.h"
// #include "mindnet/plugins/suggestion/models/Suggestion.h"
// #include "mindnet/plugins/suggestion/models/SuggestionReview.h"
// #include "mindnet/plugins/core/models/History.h"
// #include "mindnet/plugins/slipbox/models/Map.h"
// #include "mindnet/plugins/slipbox/models/Content.h"
// #include "mindnet/plugins/slipbox/models/Note.h"
// #include "mindnet/plugins/slipbox/models/Property.h"
// #include "mindnet/plugins/slipbox/models/TagType.h"
// #include "mindnet/plugins/slipbox/models/Tag.h"
// #include "mindnet/plugins/slipbox/models/Collection.h"
// #include "mindnet/plugins/slipbox/models/CollectionItem.h"
// #include "mindnet/plugins/supermemo/models/Review.h"
// #include "mindnet/plugins/supermemo/models/SM2State.h"
// #include "mindnet/plugins/slipbox/models/Question.h"
// #include "mindnet/plugins/slipbox/models/Reference.h"
// #include "mindnet/plugins/slipbox/models/Link.h"

namespace mindnet::plugins::core::models
{
    struct User;
}

namespace mindnet::plugins::core::CorePersistenceMethods
{
    std::pair<plugins::core::models::User, mindnet::api::OperationResult> find_logged_user(
        api::DbPtr& db, api::LoginToken token);

    bool has_user_name(const api::RequestContext& ctx, string user_name);
    bool has_user_email(const api::RequestContext& ctx, string user_mail);
    string is_member_of_team(const api::RequestContext& ctx, int team_id);

    // bool has_map_name(const RequestContext& ctx, string map_name);
    // std::pair<int, string> find_note_for_content(const RequestContext& ctx, int content_id);
    // bool has_right_for_map(const RequestContext& ctx, const int map_id,
    //                        const plugins::core::enums::SingleRight single_right);

    // gen_find_h(slipbox, Collection, collection)
    // gen_find_h(slipbox, CollectionItem, collection_item)
    // gen_find_h(chat, Comment, comment)
    // gen_find_h(slipbox, Content, content)
    // gen_find_h(chat, Discussion, discussion)
    // gen_find_h(core, History, history)
    // gen_find_h(slipbox, Link, link)
    // gen_find_h(slipbox, Map, map)
    // gen_find_h(mail, Message, message)
    // gen_find_h(slipbox, Note, note)
    // gen_find_h(slipbox, Property, property)
    // gen_find_h(slipbox, Question, question)
    // gen_find_h(slipbox, Reference, reference)
    // gen_find_h(supermemo, Review, review)
    // gen_find_h(supermemo, SM2State, sm2state)
    // gen_find_h(suggestion, Suggestion, suggestion)
    // gen_find_h(suggestion, SuggestionReview, suggestion_review)
    // gen_find_h(slipbox, Tag, tag)
    // gen_find_h(slipbox, TagType, tag_type)
    gen_find_h(core, Team, team)
    gen_find_h(core, TeamMember, team_member)
    gen_find_h(core, User, user)
}

#endif // COREPERSISTENCEMETHODS_H
