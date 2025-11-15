//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/TeamMemberValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/TeamMember.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/core/CorePersistenceMethods.hpp"

#define Model TeamMember
#define MODEL TEAM_MEMBER
#define model team_member

namespace mindnet::plugins::core::validators
{
    using validators::TeamMemberValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult TeamMemberValidator::validate_create_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "User does not have permission to create a team member.");

        auto team = core::find_team(ctx, entity.team_id);;
        return_if(!team.second.empty(), 400, "Team does not exist.")

        if (ctx.token.user_id == team.first.leader_id || ctx.role == mindnet::essential::UserRole::Admin)
        {
        }
        else if (ctx.token.user_id == entity.user_id)
        {
            return_if(entity.role != mindnet::essential::UserRole::Reader,
                      403, "Initial user role in team must be READER.")

            return_if(entity.status != essential::UserStatus::Pending,
                      403, "Initial user status in team must be PENDING.")
        }
        else
        {
            return {403, "You can only add members to your team."};
        }

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_read_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        auto team_member = core::find_team_member(ctx, entity.get_id());
        return_if(!team_member.second.empty(), 400, team_member.second);

        auto team = core::find_team(ctx, team_member.first.team_id);
        return_if(!team.second.empty(), 400, team.second);

        return_if(ctx.role == mindnet::essential::UserRole::Admin, 0, "")
        return_if(ctx.token.user_id == team.first.leader_id, 0, "")
        return_if(
            ctx.token.user_id == team_member.first.user_id && team_member.first.status ==
            mindnet::essential::UserStatus::Active,
            0, "")

        auto is_member = core::is_member_of_team(ctx, team.first.get_id());
        return_if(!is_member.empty(),
                  403, "You can only read your own team members.")

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_update_authorization(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        auto team = core::find_team(ctx, old_entity.team_id);
        return_if(team.second.empty(), 400, team.second)

        return_if(ctx.role != mindnet::essential::UserRole::Admin && ctx.token.user_id != team.first.leader_id,
                  403, "Only team leader can update the team.")

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        auto team_member = core::find_team_member(ctx, entity.user_id);
        return_if(team_member.second.empty(), 400, team_member.second)

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_list_authorization(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        if (ctx.role == mindnet::essential::UserRole::Admin) return ok_result;

        mandatory_filter(team_id)

        auto is_member = core::is_member_of_team(ctx, std::stoi(filter.at("team_id")));
        return_if(!is_member.empty(),
                  403, "Only team members can list team members.")

        return ok_result;
    }


    OperationResult TeamMemberValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.left_at != 0,
                  400, "left_at must be set to 0 during team member creation")

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return OperationResult(403, "Deleting team members is forbidden. Set status to DELETED.");
    }

    OperationResult TeamMemberValidator::validate_list_integrity(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        return ok_result;
    }

    string TeamMemberValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
