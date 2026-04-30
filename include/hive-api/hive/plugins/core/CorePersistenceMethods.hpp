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

#include "../../api/PersistenceMethods.hpp"
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
// #include "hive/plugins/repetition/models/Review.hpp"
// #include "hive/plugins/repetition/models/SM2State.hpp"

namespace hive::plugins::core::models
{
    struct User;
}

namespace hive::plugins::core
{
    std::pair<plugins::core::models::User, hive::api::OperationResult> find_logged_user(
        api::DbPtr& db, api::AccessTokenContext token);

    bool has_user_name(const api::RequestContext& ctx, string user_name);
    bool has_user_email(const api::RequestContext& ctx, string user_mail);
    string is_member_of_team(const api::RequestContext& ctx, identification team_id);

    gen_find_h(core, Team, team)
    gen_find_h(core, TeamMember, team_member)
    gen_find_h(core, User, user)
}