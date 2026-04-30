/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <utility>

#include "hive/api/PersistenceMethods.hpp"
#include "hive/api/IPersistence.hpp"
#include "hive/api/OperationResult.hpp"
#include "hive/api/RequestContext.hpp"
#include "hive/plugins/core/enums/SingleRight.hpp"
#include "hive/api/AccessTokenContext.hpp"
#include "hive/api/PersistenceMethods.hpp"
#include "hive/plugins/core/models/User.hpp"
// #include "hive/plugins/mail/models/Message.hpp"
#include "hive/plugins/core/models/Team.hpp"
#include "hive/plugins/core/models/TeamMember.hpp"
// #include "hive/plugins/chat/models/Discussion.hpp"
// #include "hive/plugins/chat/models/Comment.hpp"
// #include "hive/plugins/suggestion/models/Suggestion.hpp"
// #include "hive/plugins/suggestion/models/SuggestionReview.hpp"
// #include "hive/plugins/core/models/History.hpp"
#include "hive/plugins/slipbox/models/Map.hpp"
#include "hive/plugins/slipbox/models/MapCollection.hpp"
#include "hive/plugins/slipbox/models/Content.hpp"
#include "hive/plugins/slipbox/models/Note.hpp"
#include "hive/plugins/slipbox/models/Property.hpp"
#include "hive/plugins/slipbox/models/TagType.hpp"
#include "hive/plugins/slipbox/models/Tag.hpp"
#include "hive/plugins/slipbox/models/Collection.hpp"
#include "hive/plugins/slipbox/models/CollectionItem.hpp"
// #include "hive/plugins/repetition/models/Review.hpp"
// #include "hive/plugins/repetition/models/SM2State.hpp"
#include "hive/plugins/slipbox/models/Question.hpp"
#include "hive/plugins/slipbox/models/Link.hpp"
#include "hive/plugins/slipbox/models/Test.hpp"
#include "hive/plugins/slipbox/models/TestAttempt.hpp"
#include "hive/plugins/slipbox/models/Url.hpp"
#include "hive/plugins/slipbox/models/Annotation.hpp"

namespace hive::plugins::slipbox::models
{
    struct User;
}

namespace hive::plugins::slipbox
{
    bool has_map_name(const api::RequestContext& ctx, string map_name);
    std::pair<identification, string> find_note_for_content(const api::RequestContext& ctx, identification content_id);
    bool has_right_for_map(const api::RequestContext& ctx, const identification map_id,
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
    gen_find_h(slipbox, Test, test)
    gen_find_h(slipbox, TestAttempt, test_attempt)
    gen_find_h(slipbox, Property, property)
    gen_find_h(slipbox, Question, question)
    gen_find_h(slipbox, Link, link)
    // gen_find_h(repetition, R2Review, r2_review)
    // gen_find_h(repetition, R2State, r2_state)

    gen_find_h(slipbox, Tag, tag)
    gen_find_h(slipbox, TagType, tag_type)
    gen_find_h(slipbox, MapCollection, map_collection)
    gen_find_h(slipbox, Annotation, annotation)
}