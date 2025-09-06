//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/CommentCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Comment.h"
#include "mindnet/models/Discussion.h"
#include "mindnet/persistence/Persistence.h"
#include "mindnet/persistence/api/PersistenceMethods.h"

#define Model Comment
#define MODEL COMMENT
#define model comment

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::CommentCrudlValidator;

    operation_result CommentCrudlValidator::can_create(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);
        
        auto discussion = find_model(discussion, new_entity.discussion_id);
        check_found(discussion);
        auto is_team_member_result = api::is_member_of_team(db, token, discussion.first.team_id);
        return_if (!is_team_member_result.empty(), 400, "Team does not exist.");

        return_if (new_entity.user_id != token.user_id, 400, "You can only create comments for your own user.");
        
        return ok_result;
    }

    operation_result CommentCrudlValidator::can_read(db_& db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        auto comment = api::find_comment(db, token, id);
        return_if(!comment.second.empty(), 400, "Comment does not exist.")

        if (is_admin) return ok_result;

        auto discussion = api::find_discussion(db, token, comment.first.discussion_id);
        return_if(!discussion.second.empty(), 400, "Discussion does not exist.")

        auto is_team_member_result = api::is_member_of_team(db, token, discussion.first.team_id);
        return_if(!is_team_member_result.empty(), 400, "You are not a member of the team.")
        return ok_result;
    }

    operation_result CommentCrudlValidator::can_update(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        return_if (old_entity.user_id != logged_in_user.get_id() && logged_in_user.role != enums::UserRole::ADMIN,
            403, "You can only update your own comment.")

        return ok_result;
    }

    operation_result CommentCrudlValidator::can_delete(db_& db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        return operation_result(403, "Deleting comments is forbidden. Set is_deleted to true.");
    }

    operation_result CommentCrudlValidator::can_list(db_& db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)

        mandatory_filter(discussion_id)

        auto discussion = api::find_discussion(db, token, stoi(filter["discussion_id"]));
        return_if (!discussion.second.empty(),400, discussion.second)

        auto is_member_of_team_result = api::is_member_of_team(db, token, discussion.first.team_id);

        return_if (!is_member_of_team_result.empty(),403, std::string("You can only list comments for teams, you are member of. ") + is_member_of_team_result)

        return ok_result;
    }

    string CommentCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}
