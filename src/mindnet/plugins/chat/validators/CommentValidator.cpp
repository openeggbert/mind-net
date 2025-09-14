//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/plugins/chat/validators/CommentValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/chat/models/Comment.h"
#include "mindnet/plugins/chat/models/Discussion.h"
#include "mindnet/api/PersistenceMethods.h"
#include "mindnet/api/Persistence.h"


#define Model mindnet::plugins::chat::models::Comment
#define MODEL COMMENT
#define model comment

namespace mindnet::plugins::chat::validators
{
    using validators::CommentValidator;
    using mindnet::OperationResult;

    OperationResult CommentValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        auto discussion = find_model(discussion, entity.discussion_id);
        check_found(discussion);
        auto is_team_member_result = is_member_of_team(ctx, discussion.first.team_id);
        return_if(!is_team_member_result.empty(), 400, "Team does not exist.");

        return_if(entity.user_id != ctx.token.user_id, 400, "You can only create comments for your own user.");

        return ok_result;
    }

    OperationResult CommentValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto comment = find_comment(ctx, entity.get_id());
        return_if(!comment.second.empty(), 400, "Comment does not exist.")

        if (ctx.role == plugins::core::enums::UserRole::Admin) return ok_result;

        auto discussion = find_discussion(ctx, comment.first.discussion_id);
        return_if(!discussion.second.empty(), 400, "Discussion does not exist.")

        auto is_team_member_result = is_member_of_team(ctx, discussion.first.team_id);
        return_if(!is_team_member_result.empty(), 400, "You are not a member of the team.")
        return ok_result;
    }

    OperationResult CommentValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                           const Model& new_entity) const
    {
        return_if(old_entity.user_id != ctx.token.user_id && ctx.role != plugins::core::enums::UserRole::Admin,
                  403, "You can only update your own comment.")

        return ok_result;
    }

    OperationResult CommentValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        return OperationResult(403, "Deleting comments is forbidden. Set is_deleted to true.");
    }

    OperationResult CommentValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(discussion_id)

        auto discussion = find_discussion(ctx, std::stoi(filter.at("discussion_id")));
        return_if(!discussion.second.empty(), 400, discussion.second)

        auto is_member_of_team_result = is_member_of_team(ctx, discussion.first.team_id);

        return_if(!is_member_of_team_result.empty(), 403,
                  std::string("You can only list comments for teams, you are member of. ") + is_member_of_team_result)

        return ok_result;
    }

    string CommentValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
