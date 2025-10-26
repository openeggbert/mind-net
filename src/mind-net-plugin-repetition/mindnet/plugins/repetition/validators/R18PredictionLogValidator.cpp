//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/R18PredictionLogValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/R18PredictionLog.h"
#include "mindnet/api/Persistence.h"

#define Model R18PredictionLog
#define MODEL R18_PREDICTION_LOG
#define model r18_prediction_log

namespace mindnet::plugins::repetition::validators
{
    using validators::R18PredictionLogValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult R18PredictionLogValidator::validate_create_authorization(const RequestContext& ctx,
                                                                             const Model& entity) const
    {
        // User can only create prediction logs for themselves
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only create prediction logs for yourself"};
        }
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_read_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        // User can only read their own prediction logs
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only access prediction logs for yourself"};
        }
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_update_authorization(const RequestContext& ctx,
                                                                             const Model& old_entity,
                                                                             const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult R18PredictionLogValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                             const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult R18PredictionLogValidator::validate_list_authorization(const RequestContext& ctx,
                                                                           const string_map& filter) const
    {
        mandatory_filter(user_id)

        // Users can only list their own prediction logs
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoi(it->second) != ctx.token.user_id)
        {
            return {403, "Can only list your own prediction logs"};
        }
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        if (entity.predicted_R_times_100 < 0)
        {
            return {400, "predicted_R_times_100 must be non-negative"};
        }
        if (entity.actual_grade < 0)
        {
            return {400, "actual_grade must be non-negative"};
        }
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_read_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_update_integrity(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_delete_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PredictionLogValidator::validate_list_integrity(const RequestContext& ctx,
                                                                       const string_map& filter) const
    {
        return ok_result;
    }

    string R18PredictionLogValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
