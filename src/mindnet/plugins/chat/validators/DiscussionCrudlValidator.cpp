//
// Created by robertvokac on 8/6/25.
//


#include "../../../../../include/mindnet/plugins/chat/validators/DiscussionCrudlValidator.h"

#include "../../../../../include/mindnet/plugins/core/validators/UserCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/chat/models/Discussion.h"
#include "mindnet/persistence/api/Persistence.h"
#include "mindnet/persistence/api/PersistenceMethods.h"

#define Model Discussion
#define MODEL DISCUSSION
#define model discussion

namespace mindnet::plugins::chat::validators
{
    using validators::DiscussionCrudlValidator;
    using mindnet::OperationResult;

    OperationResult DiscussionCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return_if (ctx.role < plugins::core::enums::UserRole::EDITOR,
            403, "User does not have permission to create a discussion.")

        string is_member_of_team_result = is_member_of_team(ctx, entity.team_id);
        return_if (!is_member_of_team_result.empty(),
            403, "You can only create discussions for teams, you are member of. " + is_member_of_team_result);

        return_if (entity.created_by != ctx.token.user_id,
            400, "created_by must be set to the logged in user.")
        return_if (entity.is_archived,
            400, "is_archived must be set to false during discussion creation.");

        return ok_result;
    }

    OperationResult DiscussionCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {


        if (ctx.role == plugins::core::enums::UserRole::ADMIN) return ok_result;

        auto discussion = find_model(model, entity.get_id())
        check_found(discussion)

        string is_member_of_team_result = is_member_of_team(ctx, discussion.first.team_id);
        return_if (!is_member_of_team_result.empty(),
            403, "You can only create discussions for teams, you are member of. " + is_member_of_team_result);

        return ok_result;
    }

    OperationResult DiscussionCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return_if (old_entity.created_by != ctx.token.user_id,
            403, "You can only update your own discussion.")

        string is_member_of_team_result = is_member_of_team(ctx, old_entity.team_id);
        return_if (!is_member_of_team_result.empty() && ctx.role != plugins::core::enums::UserRole::ADMIN,
            403, "You can only update discussions, you created." + is_member_of_team_result);

        return ok_result;
    }

    OperationResult DiscussionCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {


        return {403, "Deleting discussions is forbidden. Set is_archived to true."};
    }

    OperationResult DiscussionCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {


        mandatory_filter(team_id)

        string is_member_of_team_result = is_member_of_team(ctx, std::stoi(filter.at("team_id")));
        return_if (!is_member_of_team_result.empty(),
            403, "You can only list discussions for teams, you are member of. " + is_member_of_team_result);

        return ok_result;
    }

    string DiscussionCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model