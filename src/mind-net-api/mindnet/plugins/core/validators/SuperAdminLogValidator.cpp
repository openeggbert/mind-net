//
// Created by robertvokac on 9/21/25.
//

#include "mindnet/plugins/core/validators/SuperAdminLogValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/SuperAdminLog.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model SuperAdminLog
#define MODEL SUPER_ADMIN_LOG
#define model super_admin_log

namespace mindnet::plugins::core::validators
{
    using validators::SuperAdminLogValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    // ===== Authorization =====
    OperationResult SuperAdminLogValidator::validate_create_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        // Only system/internal processes should create super admin logs
        return ok_result;
    }

    OperationResult SuperAdminLogValidator::validate_read_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        // Only super admins can read these logs
        return_if(ctx.role != mindnet::essential::UserRole::SuperAdmin,
                  403, "Only super administrators can access these logs.");

        return ok_result;
    }

    OperationResult SuperAdminLogValidator::validate_update_authorization(const RequestContext& ctx,
                                                                          const Model& old_entity,
                                                                          const Model& new_entity) const
    {
        return {405, "Super admin log entries cannot be updated."};
    }

    OperationResult SuperAdminLogValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        return {405, "Super admin log entries cannot be deleted."};
    }

    OperationResult SuperAdminLogValidator::validate_list_authorization(const RequestContext& ctx,
                                                                        const string_map& filter) const
    {
        // Only super admins can list these logs
        return_if(ctx.role != mindnet::essential::UserRole::SuperAdmin,
                  403, "Only super administrators can access these logs.");

        return ok_result;
    }

    // ===== Integrity =====
    OperationResult SuperAdminLogValidator::validate_create_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        // Validate required fields
        return_if(entity.endpoint.empty(), 400, "Endpoint is required");
        return_if(entity.action.empty(), 400, "Action is required");
        return_if(entity.status_code == 0, 400, "Status code is required");

        return ok_result;
    }

    OperationResult SuperAdminLogValidator::validate_read_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuperAdminLogValidator::validate_update_integrity(const RequestContext& ctx,
                                                                      const Model& old_entity,
                                                                      const Model& new_entity) const
    {
        return {405, "Super admin log entries cannot be updated."};
    }

    OperationResult SuperAdminLogValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return {405, "Super admin log entries cannot be deleted."};
    }

    OperationResult SuperAdminLogValidator::validate_list_integrity(const RequestContext& ctx,
                                                                    const string_map& filter) const
    {
        return ok_result;
    }

    string SuperAdminLogValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
