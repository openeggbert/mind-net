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
        // Only allow logged-in users to create their own performance aggregates
        if (ctx.user_id != entity.get_user_id())
        {
            return {403, "Can only create performance aggregates for yourself"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_read_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        // Users can only read their own performance data
        if (ctx.user_id != entity.get_user_id())
        {
            return {403, "Can only access your own performance aggregates"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_update_authorization(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        // Users can only update their own performance data
        if (ctx.user_id != old_entity.get_user_id() ||
            old_entity.get_user_id() != new_entity.get_user_id())
        {
            return {403, "Can only update your own performance aggregates"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        // Users can only delete their own performance data
        if (ctx.user_id != entity.get_user_id())
        {
            return {403, "Can only delete your own performance aggregates"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_list_authorization(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        // Allow listing but filtering will be handled in integrity check
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        if (entity.get_total() < 0)
        {
            return {400, "Total count cannot be negative"};
        }
        if (entity.get_correct() < 0 || entity.get_correct() > entity.get_total())
        {
            return {400, "Correct count must be between 0 and total"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        if (new_entity.get_total() < 0)
        {
            return {400, "Total count cannot be negative"};
        }
        if (new_entity.get_correct() < 0 || new_entity.get_correct() > new_entity.get_total())
        {
            return {400, "Correct count must be between 0 and total"};
        }
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R18PerfAggValidator::validate_list_integrity(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        // Ensure users can only list their own performance data
        if (filter.find("user_id") == filter.end() ||
            std::stoi(filter.at("user_id")) != ctx.user_id)
        {
            return {400, "Must filter by your own user_id"};
        }
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
