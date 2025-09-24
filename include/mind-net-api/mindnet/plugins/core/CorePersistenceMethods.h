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
// #include "mindnet/plugins/supermemo/models/Review.h"
// #include "mindnet/plugins/supermemo/models/SM2State.h"


namespace mindnet::plugins::core::models
{
    struct User;
}

namespace mindnet::plugins::core
{
    std::pair<plugins::core::models::User, mindnet::api::OperationResult> find_logged_user(
        api::DbPtr& db, api::LoginToken token);

    bool has_user_name(const api::RequestContext& ctx, string user_name);
    bool has_user_email(const api::RequestContext& ctx, string user_mail);
    string is_member_of_team(const api::RequestContext& ctx, int team_id);


    gen_find_h(core, Team, team)
    gen_find_h(core, TeamMember, team_member)
    gen_find_h(core, User, user)
}

#endif // COREPERSISTENCEMETHODS_H
