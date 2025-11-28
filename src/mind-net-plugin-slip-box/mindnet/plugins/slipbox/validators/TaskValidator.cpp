/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/slipbox/validators/TaskValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/slipbox/models/Task.hpp"
#include "mindnet/api/Persistence.hpp"

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
