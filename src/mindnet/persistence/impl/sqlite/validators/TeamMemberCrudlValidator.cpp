//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/TeamMemberCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Team.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/persistence/Persistence.h"
#include "mindnet/persistence/api/PersistenceMethods.h"

#define Model TeamMember
#define MODEL TEAM_MEMBER
#define model team_member

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TeamMemberCrudlValidator;

    operation_result TeamMemberCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if(role < enums::UserRole::EDITOR,
                  403, "User does not have permission to create a team member.");

        auto team = find_model(team, new_entity.team_id);
        return_if(!team.second.empty(), 400, "Team does not exist.")

        if (logged_in_user.get_id() == team.first.leader_id || logged_in_user.role == enums::UserRole::ADMIN)
        {
        }
        else if (logged_in_user.get_id() == new_entity.user_id)
        {
            return_if (new_entity.role != enums::UserRole::READER,
                403, "Initial user role in team must be READER.")

            return_if (new_entity.status != enums::UserStatus::PENDING,
                403, "Initial user status in team must be PENDING.")
        }
        else
        {
            return {403, "You can only add members to your team."};
        }
        return_if (new_entity.left_at != 0,
                400, "left_at must be set to 0 during team member creation")

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        auto team_member = find_model(model, entity.get_id())
        return_if(!team_member.second.empty(), 400, team_member.second);

        auto team = find_model(team, team_member.first.team_id)
        return_if(!team.second.empty(), 400, team.second);

        return_if (logged_in_user.role == enums::UserRole::ADMIN,0, "")
        return_if (logged_in_user.get_id() == team.first.leader_id,0, "")
        return_if (logged_in_user.get_id() == team_member.first.user_id && team_member.first.status == enums::UserStatus::ACTIVE,
            0, "")

        auto is_member = api::is_member_of_team(db, token, team.first.get_id());
        return_if (!is_member.empty(),
            403, "You can only read your own team members.")

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        auto team = find_model(team, old_entity.team_id)
        return_if (team.second.empty(), 400, team.second)

        return_if (!is_admin && logged_in_user.get_id() != team.first.leader_id,
            403, "Only team leader can update the team.")

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        auto team_member = find_model(model, id)
        return_if (team_member.second.empty(), 400, team_member.second)

        return operation_result(403, "Deleting team members is forbidden. Set status to DELETED.");
    }

    operation_result TeamMemberCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)

        if (is_admin) return ok_result;

        mandatory_filter(team_id)

        auto is_member = api::is_member_of_team(db, token, filter["team_id"]);
        return_if (!is_member.empty(),
            403, "Only team members can list team members.")

        return ok_result;
    }

    string TeamMemberCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}
