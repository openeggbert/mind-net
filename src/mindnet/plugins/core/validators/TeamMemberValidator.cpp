//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/TeamMemberValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/models/TeamMember.h"
#include "mindnet/api/Persistence.h"
#include "mindnet/api/PersistenceMethods.h"

#define Model TeamMember
#define MODEL TEAM_MEMBER
#define model team_member

namespace mindnet::plugins::core::validators
{
    using validators::TeamMemberValidator;
    using mindnet::OperationResult;

    OperationResult TeamMemberValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < plugins::core::enums::UserRole::Editor,
                  403, "User does not have permission to create a team member.");

        auto team = find_model(team, entity.team_id);
        return_if(!team.second.empty(), 400, "Team does not exist.")

        if (ctx.token.user_id == team.first.leader_id || ctx.role == plugins::core::enums::UserRole::Admin)
        {
        }
        else if (ctx.token.user_id == entity.user_id)
        {
            return_if(entity.role != plugins::core::enums::UserRole::Reader,
                      403, "Initial user role in team must be READER.")

            return_if(entity.status != plugins::core::enums::UserStatus::Pending,
                      403, "Initial user status in team must be PENDING.")
        }
        else
        {
            return {403, "You can only add members to your team."};
        }
        return_if(entity.left_at != 0,
                  400, "left_at must be set to 0 during team member creation")

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto team_member = find_model(model, entity.get_id())
        return_if(!team_member.second.empty(), 400, team_member.second);

        auto team = find_model(team, team_member.first.team_id)
        return_if(!team.second.empty(), 400, team.second);

        return_if(ctx.role == plugins::core::enums::UserRole::Admin, 0, "")
        return_if(ctx.token.user_id == team.first.leader_id, 0, "")
        return_if(
            ctx.token.user_id == team_member.first.user_id && team_member.first.status == plugins::core::enums::
            UserStatus::Active,
            0, "")

        auto is_member = is_member_of_team(ctx, team.first.get_id());
        return_if(!is_member.empty(),
                  403, "You can only read your own team members.")

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                              const Model& new_entity) const
    {
        auto team = find_model(team, old_entity.team_id)
        return_if(team.second.empty(), 400, team.second)

        return_if(ctx.role != plugins::core::enums::UserRole::Admin && ctx.token.user_id != team.first.leader_id,
                  403, "Only team leader can update the team.")

        return ok_result;
    }

    OperationResult TeamMemberValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        auto team_member = find_model(model, entity.user_id)
        return_if(team_member.second.empty(), 400, team_member.second)

        return OperationResult(403, "Deleting team members is forbidden. Set status to DELETED.");
    }

    OperationResult TeamMemberValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        if (ctx.role == plugins::core::enums::UserRole::Admin) return ok_result;

        mandatory_filter(team_id)

        auto is_member = is_member_of_team(ctx, std::stoi(filter.at("team_id")));
        return_if(!is_member.empty(),
                  403, "Only team members can list team members.")

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
