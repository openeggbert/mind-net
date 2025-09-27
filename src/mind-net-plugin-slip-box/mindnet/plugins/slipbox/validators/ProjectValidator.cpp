//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/ProjectValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Project.h"
#include "../../../../../../include/mind-net-api/mindnet/api/Persistence.h"

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
        if (!ctx.is_authenticated())
        {
            return {401, "Authentication required to create projects"};
        }
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
        if (ctx.user_id == entity.created_by ||
            ctx.user_id == entity.owner_id ||
            ctx.user_id == entity.assigned_to)
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
        if (ctx.user_id == old_entity.created_by ||
            ctx.user_id == old_entity.owner_id ||
            ctx.user_id == old_entity.assigned_to)
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
        if (ctx.user_id == entity.created_by ||
            ctx.user_id == entity.owner_id)
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
        // Validate progress range
        if (entity.progress < 0 || entity.progress > 100)
        {
            return {400, "Progress must be between 0 and 100"};
        }

        // Validate required fields
        if (entity.title.empty())
        {
            return {400, "Title is required"};
        }

        return ok_result;
    }

    OperationResult ProjectValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ProjectValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        // Validate progress range
        if (new_entity.progress < 0 || new_entity.progress > 100)
        {
            return {400, "Progress must be between 0 and 100"};
        }

        // Validate required fields
        if (new_entity.title.empty())
        {
            return {400, "Title is required"};
        }

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
