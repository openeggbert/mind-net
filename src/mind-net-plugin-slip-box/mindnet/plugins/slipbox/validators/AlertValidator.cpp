//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/AlertValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Alert.h"
#include "mindnet/api/Persistence.h"

#define Model Alert
#define MODEL ALERT
#define model alert

namespace mindnet::plugins::slipbox::validators
{
    using validators::AlertValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult AlertValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Only allow creating alerts for self
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Unauthorized - can only create alerts for self"};
        }
        return ok_result;
    }

    OperationResult AlertValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Only allow reading own alerts
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Unauthorized - can only read own alerts"};
        }
        return ok_result;
    }

    OperationResult AlertValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        // Only allow updating own alerts
        if (ctx.token.user_id != old_entity.user_id || ctx.token.user_id != new_entity.user_id)
        {
            return {40403, "Unauthorized - can only update own alerts"};
        }
        return ok_result;
    }

    OperationResult AlertValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Only allow deleting own alerts
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Unauthorized - can only delete own alerts"};
        }
        return ok_result;
    }

    OperationResult AlertValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        // List authorization handled by repository filter
        return ok_result;
    }

    OperationResult AlertValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AlertValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AlertValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                              const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult AlertValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AlertValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string AlertValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
