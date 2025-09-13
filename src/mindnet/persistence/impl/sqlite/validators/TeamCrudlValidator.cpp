//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/TeamCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Team.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Team
#define MODEL TEAM
#define model team

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TeamCrudlValidator;
    using api::OperationResult;

    OperationResult TeamCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {

        return_if (ctx.role < enums::UserRole::EDITOR,
            403, "User does not have permission to create a team.");

        return_if (entity.created_by != ctx.token.user_id,
                400, "created_by must be set to the logged in user.")
        return_if (entity.leader_id != ctx.token.user_id,
                400, "leader_id must be set to the logged in user.")

        return ok_result;
    }

    OperationResult TeamCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return_if (ctx.role != enums::UserRole::ADMIN && ctx.token.user_id != new_entity.leader_id,
            403, "Only team leader can update the team.")
        return_if (old_entity.created_by != new_entity.created_by,
            400, "created_by cannot be changed")

        return_if (old_entity.leader_id != new_entity.leader_id && ctx.role != enums::UserRole::ADMIN,
            400, "leader_id cannot be changed by yourself. Contact admin.")

        return ok_result;
    }

    OperationResult TeamCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {


        return_if (ctx.role != enums::UserRole::ADMIN,
            403, "Only admins can delete a team. Contact admin");

        return ok_result;
    }

    OperationResult TeamCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string TeamCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model