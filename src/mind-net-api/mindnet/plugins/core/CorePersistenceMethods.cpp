//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/plugins/core/CorePersistenceMethods.h"

#include "mindnet/api/IService.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/orm/QueryParams.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/core/models/Team.h"
#include "mindnet/plugins/core/models/User.h"

namespace mindnet::plugins::core
{
    std::pair<plugins::core::models::User, mindnet::api::OperationResult> find_logged_user(
        api::DbPtr& db, api::LoginToken token)
    {
        if (token.user_id == 0)
        {
            plugins::core::models::User u;
            u.role = essential::UserRole::Guest;
            u.status = essential::UserStatus::Active;
            return {u, ok_result};
        }
        auto result = db->read(plugins::core::models::USER_DEFINITION, token, token.user_id);
        if (result.second.ko())
        {
            return {plugins::core::models::User(), result.second};
        }
        plugins::core::models::User user;
        user.from_values(result.first);
        return {user, ok_result};
    }

    bool has_user_name(const api::RequestContext& ctx, string user_name)
    {
        string error;
        orm::QueryParams query_params;
        query_params.filters.emplace("name", user_name);

        return !ctx.db->list(plugins::core::models::USER_DEFINITION, ctx.token, query_params).first.empty();
    }

    bool has_user_email(const api::RequestContext& ctx, string user_email)
    {
        string error;
        orm::QueryParams query_params;
        query_params.filters.emplace("email", user_email);
        return !ctx.db->list(plugins::core::models::USER_DEFINITION, ctx.token, query_params).first.empty();
    }
    string is_member_of_team(const api::RequestContext& ctx, int team_id)
    {
        auto team_result = ctx.db->read(plugins::core::models::TEAM_DEFINITION, ctx.token, team_id);
        if (team_result.second.ko()) return team_result.second.error;
        plugins::core::models::Team team;
        team.from_values(team_result.first);

        orm::QueryParams query_params;
        query_params.filters.emplace("team_id", std::to_string(team.get_id()));
        query_params.filters.emplace("user_id", std::to_string(ctx.token.user_id));
        query_params.filters.emplace("status", std::to_string(cast64(essential::UserStatus::Active)));
        auto is_team_member_result = ctx.db->list(plugins::core::models::TEAM_MEMBER_DEFINITION, ctx.token,
                                                  query_params);
        if (is_team_member_result.second.ko()) return is_team_member_result.second.error;
        if (is_team_member_result.first.empty())
        {
            return "User is not member of team with id " + std::to_string(team.get_id()) + ".";
        }
        return "";
    }


    // gen_find_cpp(chat, Comment, comment, COMMENT)
    gen_find_cpp(core, User, user, USER)
    // gen_find_cpp(mail, Message, message, MESSAGE)
    gen_find_cpp(core, Team, team, TEAM)
    gen_find_cpp(core, TeamMember, team_member, TEAM_MEMBER)

}
