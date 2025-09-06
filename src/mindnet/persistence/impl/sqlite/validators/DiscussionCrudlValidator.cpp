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
#include "mindnet/persistence/api/PersistenceMethods.h"

#define Model Discussion
#define MODEL DISCUSSION
#define model discussion

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::UserCrudlValidator;

    operation_result DiscussionCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (logged_in_user.role < enums::UserRole::EDITOR,
            403, "User does not have permission to create a discussion.")

        string is_member_of_team_result = api::is_member_of_team(db, token, new_entity.team_id);
        return_if (!is_member_of_team_result.empty(),
            403, "You can only create discussions for teams, you are member of. " + is_member_of_team_result);

        return_if (new_entity.created_by != logged_in_user.get_id(),
            400, "created_by must be set to the logged in user.")
        return_if (new_entity.is_archived,
            400, "is_archived must be set to false during discussion creation.");

        return ok_result;
    }

    operation_result DiscussionCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        if (is_admin) return ok_result;

        auto discussion = find_model(model, id)
        check_found(discussion)

        string is_member_of_team_result = api::is_member_of_team(db, token, discussion.first.team_id);
        return_if (!is_member_of_team_result.empty(),
            403, "You can only create discussions for teams, you are member of. " + is_member_of_team_result);

        return ok_result;
    }

    operation_result DiscussionCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        return_if (old_entity.created_by != logged_in_user.get_id(),
            403, "You can only update your own discussion.")

        string is_member_of_team_result = api::is_member_of_team(db, token, old_entity.team_id);
        return_if (!is_member_of_team_result.empty() && logged_in_user.role != enums::UserRole::ADMIN,
            403, "You can only update discussions, you created." + is_member_of_team_result);

        return ok_result;
    }

    operation_result DiscussionCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        return {403, "Deleting discussions is forbidden. Set is_archived to true."};
    }

    operation_result DiscussionCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)

        mandatory_filter(team_id)

        string is_member_of_team_result = api::is_member_of_team(db, token, stoi(filter["team_id"]));
        return_if (!is_member_of_team_result.empty(),
            403, "You can only list discussions for teams, you are member of. " + is_member_of_team_result);

        return ok_result;
    }

    string DiscussionCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model