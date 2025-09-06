//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/TeamMemberCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Team.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TeamMemberCrudlValidator;
    operation_result TeamMemberCrudlValidator::can_create(db_& d, entity_fields& ef, http::LoginToken& login_token) const
    {
        //2. Authorization
        logged_user()

        //
        if (logged_in_user.role < enums::UserRole::EDITOR)
        {
            return operation_result(403, "User does not have permission to create a team member.");
        }

        //3. Request
        models::TeamMember new_entity;
        new_entity.from_values(ef);
        err << new_entity << commit;

        auto team_result = d->read(new_entity.team_id, models::TEAM_DEFINITION, login_token);
        if (team_result.second.ko()) return team_result.second;
        models::Team team;
        team.from_values(team_result.first);

        auto team_leader_result = d->read(team.leader_id, models::USER_DEFINITION, login_token);
        if (team_leader_result.second.ko()) return team_leader_result.second;
        models::User team_leader;
        team_leader.from_values(team_leader_result.first);


        if (logged_in_user.get_id() == team_leader.get_id() || logged_in_user.role == enums::UserRole::ADMIN)
        {
        }
        else if (logged_in_user.get_id() == new_entity.user_id)
        {
            if (new_entity.role != enums::UserRole::READER)
            {
                return operation_result(403, "Initial user role in team must be READER.");
            }
            if (new_entity.status != enums::UserStatus::PENDING)
            {
                return operation_result(403, "Initial user status in team must be PENDING.");
            }
        }
        else
        {
            return operation_result(403, "You can only add members to your team.");
        }
        if (new_entity.left_at != 0)
            return operation_result(
                400, "left_at must be set to 0 during team member creation");

        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::can_read(db_ d, int id, http::LoginToken& login_token) const
    {
        logged_user()
        if (logged_in_user.role == enums::UserRole::ADMIN) return ok_result;


        auto team_member_result = d->read(id, models::TEAM_MEMBER_DEFINITION, login_token);
        if (team_member_result.second.ko()) return team_member_result.second;
        models::TeamMember team_member;
        team_member.from_values(team_member_result.first);

        auto team_result = d->read(team_member.team_id, models::TEAM_DEFINITION, login_token);
        if (team_result.second.ko()) return team_result.second;
        models::Team team;
        team.from_values(team_result.first);

        // auto team_leader_result = d->read(team.leader_id, models::USER_DEFINITION, login_token);
        // if (team_leader_result.second.ko()) return team_leader_result.second;
        // models::User team_leader;
        // team_leader.from_values(team_leader_result.first);

        if (logged_in_user.get_id() == team.leader_id) return ok_result;
        if (logged_in_user.get_id() == team_member.user_id && team_member.status == enums::UserStatus::ACTIVE)
            return
                ok_result;

        http::QueryParams query_params;
        query_params.filters.emplace("team_id", std::to_string(team.get_id()));
        query_params.filters.emplace("user_id", std::to_string(logged_in_user.get_id()));
        query_params.filters.emplace("status", std::to_string(cast64(enums::UserStatus::ACTIVE)));
        auto is_team_member_result = d->list(query_params, models::TEAM_MEMBER_DEFINITION, login_token);
        if (is_team_member_result.second.ko()) return is_team_member_result.second;
        if (is_team_member_result.first.empty())
        {
            return operation_result(403, "You can only read your own team members.");
        }

        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::can_update(db_ d, entity_fields& ef, http::LoginToken& login_token) const
    {
        logged_user()

        models::TeamMember new_entity;
        new_entity.from_values(ef);
        models::TeamMember old_entity;
        auto old_entity_values = d->read(new_entity.get_id(), models::TEAM_MEMBER_DEFINITION, login_token).first;
        old_entity.from_values(old_entity_values);


        auto team_result = d->read(old_entity.team_id, models::TEAM_DEFINITION, login_token);
        if (team_result.second.ko()) return team_result.second;
        models::Team team;
        team.from_values(team_result.first);

        if (logged_in_user.role != enums::UserRole::ADMIN && logged_in_user.get_id() != team.leader_id)
        {
            return operation_result(403, "Only team leader can update the team.");
        }
        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);
        error = validate_readonly(old_entity_values, ef, models::TEAM_MEMBER_DEFINITION);
        if (!error.empty()) return operation_result(400, error);


        return ok_result;
    }

    operation_result TeamMemberCrudlValidator::can_delete(db_ d, int id, http::LoginToken& login_token) const
    {
        auto team_member_result = d->read(id, models::TEAM_MEMBER_DEFINITION, login_token);
        if (team_member_result.second.ko()) return team_member_result.second;

        return operation_result(403, "Deleting team members is forbidden. Set status to DELETED.");
    }

    operation_result TeamMemberCrudlValidator::can_list(db_ d, std::map<std::string, std::string>& filter,
                                                        http::LoginToken& login_token) const
    {
        //2. Authorization
        logged_user()

        if (logged_in_user.role == enums::UserRole::ADMIN) return ok_result;

        if (filter.find("team_id") == filter.end()) return {403, "You can't filter without team_id."};

        http::QueryParams query_params;
        query_params.filters.emplace("user_id", std::to_string(logged_in_user.get_id()));
        query_params.filters.emplace("status", std::to_string(cast64(enums::UserStatus::ACTIVE)));
        query_params.filters.emplace("team_id", filter["team_id"]);
        auto member_of_teams_result = d->list(query_params, models::TEAM_MEMBER_DEFINITION, login_token);
        if (member_of_teams_result.second.ko()) return member_of_teams_result.second;
        if (member_of_teams_result.first.empty())
        {
            return operation_result(403, "Only team members can list team members.");
        }

        return ok_result;
    }

    string TeamMemberCrudlValidator::get_model_name() const
    {
        return "team_member";
    }
}
