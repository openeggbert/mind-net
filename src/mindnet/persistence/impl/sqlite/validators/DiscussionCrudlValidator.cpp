//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/DiscussionCrudlValidator.h"

#include "mindnet/persistence/impl/sqlite/validators/UserCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Discussion.h"
#include "mindnet/models/Team.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::UserCrudlValidator;

    string is_member_of_team(db_& d, int team_id, http::LoginToken& login_token)
    {
        auto team_result = d->read(team_id, models::TEAM_DEFINITION, login_token);
        if (team_result.second.ko()) return team_result.second.error;
        models::Team team;
        team.from_values(team_result.first);

        http::QueryParams query_params;
        query_params.filters.emplace("team_id", std::to_string(team.get_id()));
        query_params.filters.emplace("user_id", std::to_string(login_token.user_id));
        query_params.filters.emplace("status", std::to_string(cast64(enums::UserStatus::ACTIVE)));
        auto is_team_member_result = d->list(query_params, models::TEAM_MEMBER_DEFINITION, login_token);
        if (is_team_member_result.second.ko()) return is_team_member_result.second.error;
        if (is_team_member_result.first.empty())
        {
            return "User is not member of team with id " + std::to_string(team.get_id()) + ".";
        }
        return "";
    }

    operation_result DiscussionCrudlValidator::can_create(db_& d, entity_fields& ef,
                                                          http::LoginToken& login_token) const
    {
        //2. Authorization
        logged_user()

        //
        if (logged_in_user.role < enums::UserRole::EDITOR)
        {
            return operation_result(403, "User does not have permission to create a discussion.");
        }

        //3. Request
        models::Discussion new_entity;
        new_entity.from_values(ef);
        err << new_entity << commit;

        string is_member_of_team_result = is_member_of_team(d, new_entity.team_id, login_token);
        if (!is_member_of_team_result.empty())
        {
            return operation_result(
                403, "You can only create discussions for teams, you are member of. " + is_member_of_team_result);
        }

        if (new_entity.created_by != logged_in_user.get_id())
            return operation_result(
                400, "created_by must be set to the logged in user.");
        if (new_entity.is_archived)
            return operation_result(
                400, "is_archived must be set to false during discussion creation.");

        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);

        return ok_result;
    }

    operation_result DiscussionCrudlValidator::can_read(db_ d, int id, http::LoginToken& login_token) const
    {
        logged_user()
        if (logged_in_user.role == enums::UserRole::ADMIN) return ok_result;


        auto discussion_result = d->read(id, models::DISCUSSION_DEFINITION, login_token);
        if (discussion_result.second.ko()) return discussion_result.second;
        models::Discussion discussion;
        discussion.from_values(discussion_result.first);


        string is_member_of_team_result = is_member_of_team(d, discussion.team_id, login_token);
        if (!is_member_of_team_result.empty())
        {
            return operation_result(
                403, "You can only create discussions for teams, you are member of. " + is_member_of_team_result);
        }

        return ok_result;
    }

    operation_result DiscussionCrudlValidator::can_update(db_ d, entity_fields& ef, http::LoginToken& login_token) const
    {
        logged_user()

        models::Discussion new_entity;
        new_entity.from_values(ef);
        models::Discussion old_entity;
        auto old_entity_values = d->read(new_entity.get_id(), models::DISCUSSION_DEFINITION, login_token).first;
        old_entity.from_values(old_entity_values);

        if (old_entity.created_by != logged_in_user.get_id()) return operation_result(403, "You can only update your own discussion.");

        string is_member_of_team_result = is_member_of_team(d, old_entity.team_id, login_token);
        if (!is_member_of_team_result.empty())
        {
            return operation_result(
                403, "You can only update discussions, you created." + is_member_of_team_result);
        }

        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);
        error = validate_readonly(old_entity_values, ef, models::TEAM_MEMBER_DEFINITION);
        if (!error.empty()) return operation_result(400, error);


        return ok_result;
    }

    operation_result DiscussionCrudlValidator::can_delete(db_ d, int id, http::LoginToken& login_token) const
    {
        auto discussion_result = d->read(id, models::DISCUSSION_DEFINITION, login_token);
        if (discussion_result.second.ko()) return discussion_result.second;

        return operation_result(403, "Deleting discussions is forbidden. Set is_archived to true.");    }

    operation_result DiscussionCrudlValidator::can_list(db_ d, std::map<std::string, std::string>& filter,
                                                        http::LoginToken& login_token) const
    {
        //2. Authorization
        logged_user()

        if (filter.find("team_id") == filter.end()) return {403, "You can't filter without team_id."};

        string is_member_of_team_result = is_member_of_team(d, stoi(filter["team_id"]), login_token);
        if (!is_member_of_team_result.empty())
        {
            return operation_result(
                403, "You can only list discussions for teams, you are member of. " + is_member_of_team_result);
        }

        return ok_result;    }

    string DiscussionCrudlValidator::get_model_name() const
    {
        return "discussion";
    }
}
