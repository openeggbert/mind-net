//
// Created by robertvokac on 9/21/25.
//

#include "mindnet/other/plugins/core/validators/ApiLogValidator.h"

#include "mindnet/core/Global.h"
#include "mindnet/other/plugins/core/models/ApiLog.h"
#include "mindnet/other/api/Persistence.h"

#define Model ApiLog
#define MODEL API_LOG
#define model api_log

namespace mindnet::plugins::core::validators
{
    using validators::ApiLogValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;

    // ===== Authorization =====
    OperationResult ApiLogValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ApiLogValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.role == plugins::core::enums::UserRole::Admin) return ok_result;

        // Users can read only their own logs
        return_if(entity.user_id != ctx.token.user_id,
                  403, "You can only read your own API logs.");

        return ok_result;
    }

    OperationResult ApiLogValidator::validate_update_authorization(const RequestContext& ctx,
                                                                   const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        return {405, "API log entries cannot be updated."};
    }

    OperationResult ApiLogValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "API log entries cannot be deleted."};
    }

    OperationResult ApiLogValidator::validate_list_authorization(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        if (ctx.role == plugins::core::enums::UserRole::Admin) return ok_result;

        // normal users may only list their own logs
        mandatory_filter(user_id)

        return_if(filter.at("user_id") != std::to_string(ctx.token.user_id),
                  403, "You can only list your own API logs.");

        return ok_result;
    }

    // ===== Integrity =====
    OperationResult ApiLogValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ApiLogValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // Authorization already checked user_id vs ctx.token.user_id
        return ok_result;
    }

    OperationResult ApiLogValidator::validate_update_integrity(const RequestContext& ctx,
                                                               const Model& old_entity,
                                                               const Model& new_entity) const
    {
        return {405, "API log entries cannot be updated."};
    }

    OperationResult ApiLogValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "API log entries cannot be deleted."};
    }

    OperationResult ApiLogValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        // Authorization already enforces user_id filter
        return ok_result;
    }

    string ApiLogValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
