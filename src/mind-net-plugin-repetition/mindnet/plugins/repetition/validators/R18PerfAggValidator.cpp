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


#include "mindnet/plugins/repetition/validators/R18PerfAggValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/repetition/models/R18PerfAgg.hpp"
#include "mindnet/api/Persistence.hpp"


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
        if (it == filter.end() || std::stoll(it->second) != ctx.token.user_id)
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
