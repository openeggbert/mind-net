//
// Created by robertvokac on 9/6/25.
//
#ifndef SLIPBOXPERSISTENCEMETHODS_H
#define SLIPBOXPERSISTENCEMETHODS_H


#include <utility>

#include "mindnet/api/PersistenceMethods.hpp"
#include "mindnet/api/IPersistence.hpp"
#include "mindnet/api/OperationResult.hpp"
#include "mindnet/api/RequestContext.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/api/PersistenceMethods.hpp"
#include "mindnet/plugins/core/models/User.hpp"
// #include "mindnet/plugins/mail/models/Message.hpp"
#include "mindnet/plugins/core/models/Team.hpp"
#include "mindnet/plugins/core/models/TeamMember.hpp"
// #include "mindnet/plugins/chat/models/Discussion.hpp"
// #include "mindnet/plugins/chat/models/Comment.hpp"
// #include "mindnet/plugins/suggestion/models/Suggestion.hpp"
// #include "mindnet/plugins/suggestion/models/SuggestionReview.hpp"
// #include "mindnet/plugins/core/models/History.hpp"
#include "mindnet/plugins/slipbox/models/Map.hpp"
#include "mindnet/plugins/slipbox/models/MapCollection.hpp"
#include "mindnet/plugins/slipbox/models/Content.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/plugins/slipbox/models/Property.hpp"
#include "mindnet/plugins/slipbox/models/TagType.hpp"
#include "mindnet/plugins/slipbox/models/Tag.hpp"
#include "mindnet/plugins/slipbox/models/Collection.hpp"
#include "mindnet/plugins/slipbox/models/CollectionItem.hpp"
// #include "mindnet/plugins/repetition/models/Review.hpp"
// #include "mindnet/plugins/repetition/models/SM2State.hpp"
#include "mindnet/plugins/slipbox/models/Question.hpp"
#include "mindnet/plugins/slipbox/models/Link.hpp"
#include "mindnet/plugins/slipbox/models/Url.hpp"

namespace mindnet::plugins::slipbox::models
{
    struct User;
}

namespace mindnet::plugins::slipbox
{
    bool has_map_name(const api::RequestContext& ctx, string map_name);
    std::pair<int, string> find_note_for_content(const api::RequestContext& ctx, int content_id);
    bool has_right_for_map(const api::RequestContext& ctx, const int map_id,
                           const plugins::core::enums::SingleRight single_right);

    gen_find_h(slipbox, Collection, collection)
    gen_find_h(slipbox, CollectionItem, collection_item)
    // gen_find_h(chat, Comment, comment)
    gen_find_h(slipbox, Content, content)
    // gen_find_h(chat, Discussion, discussion)
    // gen_find_h(core, History, history)
    gen_find_h(slipbox, Link, link)
    gen_find_h(slipbox, Map, map)
    // gen_find_h(mail, Message, message)
    gen_find_h(slipbox, Note, note)
    gen_find_h(slipbox, Property, property)
    gen_find_h(slipbox, Question, question)
    gen_find_h(slipbox, Link, link)
    // gen_find_h(repetition, R2Review, r2_review)
    // gen_find_h(repetition, R2State, r2_state)

    gen_find_h(slipbox, Tag, tag)
    gen_find_h(slipbox, TagType, tag_type)
    gen_find_h(slipbox, MapCollection, map_collection)
}

#endif // SLIPBOXPERSISTENCEMETHODS_H
