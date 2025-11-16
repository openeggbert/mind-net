//
// Created by robertvokac on 9/6/25.
//
#pragma once


#include <utility>

#include "../../api/PersistenceMethods.hpp"
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
// #include "mindnet/plugins/repetition/models/Review.hpp"
// #include "mindnet/plugins/repetition/models/SM2State.hpp"


namespace mindnet::plugins::core::models
{
    struct User;
}

namespace mindnet::plugins::core
{
    std::pair<plugins::core::models::User, mindnet::api::OperationResult> find_logged_user(
        api::DbPtr& db, api::AccessTokenContext token);

    bool has_user_name(const api::RequestContext& ctx, string user_name);
    bool has_user_email(const api::RequestContext& ctx, string user_mail);
    string is_member_of_team(const api::RequestContext& ctx, int team_id);


    gen_find_h(core, Team, team)
    gen_find_h(core, TeamMember, team_member)
    gen_find_h(core, User, user)
}

