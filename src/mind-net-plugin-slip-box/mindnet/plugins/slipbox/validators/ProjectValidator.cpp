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

#include "mindnet/plugins/slipbox/validators/ProjectValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/slipbox/models/Project.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model Project
#define MODEL PROJECT
#define model project

namespace mindnet::plugins::slipbox::validators
{
    using validators::ProjectValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult ProjectValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        // Only logged in users can create projects
        assert_editor()
        return ok_result;
    }

    OperationResult ProjectValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Public projects can be read by anyone
        if (entity.is_public)
        {
            return ok_result;
        }

        // Private projects can only be read by:
        // - The creator
        // - The owner
        // - The assigned user
        if (ctx.token.user_id == entity.created_by ||
            ctx.token.user_id == entity.owner_id ||
            ctx.token.user_id == entity.assigned_to)
        {
            return ok_result;
        }

        return {403, "No permission to access this project"};
    }

    OperationResult ProjectValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        // Only allow updates by:
        // - The creator
        // - The owner
        // - The assigned user
        if (ctx.token.user_id == old_entity.created_by ||
            ctx.token.user_id == old_entity.owner_id ||
            ctx.token.user_id == old_entity.assigned_to)
        {
            return ok_result;
        }

        return {403, "No permission to update this project"};
    }

    OperationResult ProjectValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        // Only allow deletion by:
        // - The creator
        // - The owner
        if (ctx.token.user_id == entity.created_by ||
            ctx.token.user_id == entity.owner_id)
        {
            return ok_result;
        }

        return {403, "No permission to delete this project"};
    }

    OperationResult ProjectValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        // Anyone can list projects, but results will be filtered based on permissions
        return ok_result;
    }

    OperationResult ProjectValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ProjectValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ProjectValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult ProjectValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ProjectValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string ProjectValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model