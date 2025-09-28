//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TaskValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Task.h"
#include "../../../../../../include/mind-net-api/mindnet/api/Persistence.h"

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
        // // Only logged in users can create tasks
        // if (!ctx.is_authenticated())
        // {
        //     return {ERROR_UNAUTHORIZED, "Authentication required to create tasks"};
        // }
        return ok_result;
    }

    OperationResult TaskValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // // Public tasks can be read by anyone
        // if (entity.is_public)
        {
            return ok_result;
        }

        // // Private tasks can only be read by the owner, creator, or assigned user
        // if (!ctx.is_authenticated() ||
        //     (ctx.user_id != entity.owner_id &&
        //         ctx.user_id != entity.created_by &&
        //         ctx.user_id != entity.assigned_to))
        // {
        //     return {ERROR_FORBIDDEN, "No permission to read this task"};
        // }
        return ok_result;
    }

    OperationResult TaskValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        // // Only owner, creator or assigned user can update
        // if (!ctx.is_authenticated() ||
        //     (ctx.user_id != old_entity.owner_id &&
        //         ctx.user_id != old_entity.created_by &&
        //         ctx.user_id != old_entity.assigned_to))
        // {
        //     return {ERROR_FORBIDDEN, "No permission to update this task"};
        // }
        return ok_result;
    }

    OperationResult TaskValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // // Only owner or creator can delete
        // if (!ctx.is_authenticated() ||
        //     (ctx.user_id != entity.owner_id &&
        //         ctx.user_id != entity.created_by))
        // {
        //     return {ERROR_FORBIDDEN, "No permission to delete this task"};
        // }
        return ok_result;
    }

    OperationResult TaskValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return ok_result; // List filtering will be handled at query level
    }

    OperationResult TaskValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // // Validate required fields
        // if (entity.title.empty())
        // {
        //     return {ERROR_VALIDATION, "Title is required"};
        // }
        //
        // // Validate progress range
        // if (entity.progress < 0 || entity.progress > 100)
        // {
        //     return {ERROR_VALIDATION, "Progress must be between 0 and 100"};
        // }
        //
        // // Validate dates
        // if (entity.due_date && entity.start_date && entity.due_date < entity.start_date)
        // {
        //     return {ERROR_VALIDATION, "Due date cannot be before start date"};
        // }

        return ok_result;
    }

    OperationResult TaskValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TaskValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        // // Same validations as create
        // auto create_result = validate_create_integrity(ctx, new_entity);
        // if (!create_result.is_ok())
        // {
        //     return create_result;
        // }
        //
        // // Additional update-specific validations
        // if (new_entity.status == TASK_STATUS_COMPLETED && !new_entity.completed_at)
        // {
        //     return {ERROR_VALIDATION, "Completed tasks must have a completion date"};
        // }

        return ok_result;
    }

    OperationResult TaskValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // // Check if task has dependent tasks
        // if (has_dependent_tasks(entity.id))
        // {
        //     return {ERROR_VALIDATION, "Cannot delete task with dependent tasks"};
        // }

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
