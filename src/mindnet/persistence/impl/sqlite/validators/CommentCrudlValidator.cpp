//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/CommentCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Comment.h"
#include "mindnet/models/Discussion.h"
#include "mindnet/persistence/Persistence.h"
#include "mindnet/persistence/api/PersistenceMethods.h"

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::CommentCrudlValidator;


    operation_result CommentCrudlValidator::can_create(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        models::Comment comment;
        comment.from_values(ef);

        auto discussion = api::find_discussion(d, token, comment.discussion_id);
        if (!discussion.second.empty()) return {400, "Discussion does not exist."};
        auto is_team_member_result = api::is_member_of_team(d, token, discussion.first.team_id);
        if (!is_team_member_result.empty()) return {400, "Team does not exist."};

        if (comment.user_id != token.user_id) return {400, "You can only create comments for your own user."};

        string error = comment.validate();
        if (!error.empty()) return operation_result(400, error);

        return ok_result;
    }

    operation_result CommentCrudlValidator::can_read(db_& db, http::LoginToken& token, int id) const
    {
        logged_user()

        auto comment = api::find_comment(d, token, id);
        if (!comment.second.empty()) return {400, "Comment does not exist."};

        if (logged_in_user.role == enums::UserRole::ADMIN) return ok_result;

        auto discussion = api::find_discussion(d, token, comment.first.discussion_id);
        if (!discussion.second.empty()) return {400, "Discussion does not exist."};

        auto is_team_member_result = api::is_member_of_team(d, token, discussion.first.team_id);
        if (!is_team_member_result.empty()) return {400, "You are not a member of the team."};

        return ok_result;
    }

    operation_result CommentCrudlValidator::can_update(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        logged_user()

        models::Comment new_entity;
        new_entity.from_values(ef);
        models::Comment old_entity;
        auto old_entity_values = d->read(new_entity.get_id(), models::DISCUSSION_DEFINITION, token).first;
        old_entity.from_values(old_entity_values);

        if (old_entity.user_id != logged_in_user.get_id() && logged_in_user.role != enums::UserRole::ADMIN) return
            operation_result(403, "You can only update your own comment.");

        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);
        error = validate_readonly(old_entity_values, ef, models::COMMENT_DEFINITION);
        if (!error.empty()) return operation_result(400, error);

        return ok_result;
    }

    operation_result CommentCrudlValidator::can_delete(db_& db, http::LoginToken& token, int id) const
    {
        auto entity = api::find_comment(d, token, id);

        if (!entity.second.empty()) return {400, entity.second};

        return operation_result(403, "Deleting comments is forbidden. Set is_deleted to true.");
    }

    operation_result CommentCrudlValidator::can_list(db_& db, http::LoginToken& token, string_map& filter) const
    {
        //2. Authorization
        logged_user()

        if (filter.find("discussion_id") == filter.end()) return {403, "You can't filter without discussion_id."};

        auto discussion = api::find_discussion(d, token, stoi(filter["discussion_id"]));
        if (!discussion.second.empty()) return {400, discussion.second};

        auto is_member_of_team_result = api::is_member_of_team(d, token, discussion.first.team_id);

        if (!is_member_of_team_result.empty()) return {403, "You can only list comments for teams, you are member of. " + is_member_of_team_result};

        return ok_result;
    }

    string CommentCrudlValidator::get_model_name() const
    {
        return "comment";
    }
}
