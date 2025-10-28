//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TaskValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Task.h"
#include "mindnet/api/Persistence.h"

#define Model Task
#define MODEL TASK
#define model task

namespace mindnet::plugins::slipbox::validators
{
    using validators::TaskValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult TaskValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()

        return ok_result;
    }

    OperationResult TaskValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // // Public tasks can be read by anyone
        if (entity.is_public)
        {
            return ok_result;
        }

        // Private tasks can only be read by the owner, creator, or assigned user
        if (
            ctx.token.user_id != entity.owner_id &&
            ctx.token.user_id != entity.created_by &&
            ctx.token.user_id != entity.assigned_to)
        {
            return {403, "No permission to read this task"};
        }
        return ok_result;
    }

    OperationResult TaskValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        // Only owner, creator or assigned user can update
        if (ctx.token.user_id != old_entity.owner_id &&
            ctx.token.user_id != old_entity.created_by &&
            ctx.token.user_id != old_entity.assigned_to)
        {
            return {403, "No permission to update this task"};
        }
        return ok_result;
    }

    OperationResult TaskValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Only owner or creator can delete
        if (ctx.token.user_id != entity.owner_id &&
            ctx.token.user_id != entity.created_by)
        {
            return {403, "No permission to delete this task"};
        }
        return ok_result;
    }

    OperationResult TaskValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return ok_result; // List filtering will be handled at query level
    }

    OperationResult TaskValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TaskValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TaskValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TaskValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TaskValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string TaskValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }

    // bool has_dependent_tasks(int64_t task_id) const
    // {
    //     // Implementation to check for tasks that depend on this one
    //     // through parent_task_id or blocked_by_task_id
    //     return false; // TODO: Implement actual check
    // }
}

#undef Model
#undef MODEL
#undef model
