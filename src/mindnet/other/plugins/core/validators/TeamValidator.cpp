//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/other/plugins/core/validators/TeamValidator.h"

#include "mindnet/core/Global.h"
#include "mindnet/other/plugins/core/models/Team.h"
#include "mindnet/other/api/Persistence.h"

#define Model Team
#define MODEL TEAM
#define model team

namespace mindnet::plugins::core::validators
{
    using validators::TeamValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;
    OperationResult TeamValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult TeamValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < plugins::core::enums::UserRole::Editor,
                  403, "User does not have permission to create a team.");

        return_if(entity.created_by != ctx.token.user_id,
                  400, "created_by must be set to the logged in user.")
        return_if(entity.leader_id != ctx.token.user_id,
                  400, "leader_id must be set to the logged in user.")

        return ok_result;
    }

    OperationResult TeamValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                        const Model& new_entity) const
    {
        return_if(ctx.role != plugins::core::enums::UserRole::Admin && ctx.token.user_id != new_entity.leader_id,
                  403, "Only team leader can update the team.")
        return_if(old_entity.created_by != new_entity.created_by,
                  400, "created_by cannot be changed")

        return_if(old_entity.leader_id != new_entity.leader_id && ctx.role != plugins::core::enums::UserRole::Admin,
                  400, "leader_id cannot be changed by yourself. Contact admin.")

        return ok_result;
    }

    OperationResult TeamValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role != plugins::core::enums::UserRole::Admin,
                  403, "Only admins can delete a team. Contact admin");

        return ok_result;
    }

    OperationResult TeamValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string TeamValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
