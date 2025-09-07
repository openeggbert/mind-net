//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/TeamMemberCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/persistence/api/Persistence.h"
#include "mindnet/persistence/api/PersistenceMethods.h"

#define Model TeamMember
#define MODEL TEAM_MEMBER
#define model team_member

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TeamMemberCrudlValidator;

    operation_result TeamMemberCrudlValidator::validate_create(const ValidatorContext& ctx, const Model& entity) const
    {


        return_if(ctx.logged_user.role < enums::UserRole::EDITOR,
                  403, "User does not have permission to create a team member.");

        auto team = find_model(team, entity.team_id);
        return_if(!team.second.empty(), 400, "Team does not exist.")

        if (ctx.logged_user.get_id() == team.first.leader_id || ctx.logged_user.role == enums::UserRole::ADMIN)
        {
        }
        else if (ctx.logged_user.get_id() == entity.user_id)
        {
            return_if (entity.role != enums::UserRole::READER,
                403, "Initial user role in team must be READER.")

            return_if (entity.status != enums::UserStatus::PENDING,
                403, "Initial user status in team must be PENDING.")
        }
        else
        {
            return {403, "You can only add members to your team."};
        }
        return_if (entity.left_at != 0,
                400, "left_at must be set to 0 during team member creation")

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::validate_read(const ValidatorContext& ctx, const Model& entity) const
    {


        auto team_member = find_model(model, entity.get_id())
        return_if(!team_member.second.empty(), 400, team_member.second);

        auto team = find_model(team, team_member.first.team_id)
        return_if(!team.second.empty(), 400, team.second);

        return_if (ctx.logged_user.role == enums::UserRole::ADMIN,0, "")
        return_if (ctx.logged_user.get_id() == team.first.leader_id,0, "")
        return_if (ctx.logged_user.get_id() == team_member.first.user_id && team_member.first.status == enums::UserStatus::ACTIVE,
            0, "")

        auto is_member = api::is_member_of_team(ctx, team.first.get_id());
        return_if (!is_member.empty(),
            403, "You can only read your own team members.")

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::validate_update(const ValidatorContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        auto team = find_model(team, old_entity.team_id)
        return_if (team.second.empty(), 400, team.second)

        return_if (ctx.logged_user.role != enums::UserRole::ADMIN && ctx.logged_user.get_id() != team.first.leader_id,
            403, "Only team leader can update the team.")

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::validate_delete(const ValidatorContext& ctx, const Model& entity)  const
    {


        auto team_member = find_model(model, entity.user_id)
        return_if (team_member.second.empty(), 400, team_member.second)

        return operation_result(403, "Deleting team members is forbidden. Set status to DELETED.");
    }

    operation_result TeamMemberCrudlValidator::validate_list(const ValidatorContext& ctx, const string_map& filter) const
    {

        if (ctx.logged_user.role == enums::UserRole::ADMIN) return ok_result;

        mandatory_filter(team_id)

        auto is_member = api::is_member_of_team(ctx, std::stoi(filter.at("team_id")));
        return_if (!is_member.empty(),
            403, "Only team members can list team members.")

        return ok_result;
    }

    string TeamMemberCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model