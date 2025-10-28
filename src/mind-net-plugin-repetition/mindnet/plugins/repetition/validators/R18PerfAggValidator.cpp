//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/R18PerfAggValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/R18PerfAgg.h"
#include "mindnet/api/Persistence.h"

#define Model R18PerfAgg
#define MODEL R18_PERF_AGG
#define model r18_perf_agg

namespace mindnet::plugins::repetition::validators
{
    using validators::R18PerfAggValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult R18PerfAggValidator::validate_create_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        // User can only create states for themselves
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only create perf agg for yourself"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_read_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        // User can only create states for themselves
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only access perf agg for yourself"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_update_authorization(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        // User can only create states for themselves
        if (ctx.token.user_id != new_entity.user_id)
        {
            return {403, "Can only update perf agg for yourself"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return status_405_unsupported_operation
    }

    OperationResult R18PerfAggValidator::validate_list_authorization(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        mandatory_filter(user_id)

        // Users can only list their own perf aggs
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoi(it->second) != ctx.token.user_id)
        {
            return {403, "Can only list your own perf aggs"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_list_integrity(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        return ok_result;
    }

    string R18PerfAggValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
