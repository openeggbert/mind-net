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


#include "mindnet/plugins/repetition/validators/R18PredictionLogValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/repetition/models/R18PredictionLog.hpp"
#include "mindnet/api/Persistence.hpp"


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
        if (it == filter.end() || std::stoll(it->second) != ctx.token.user_id)
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
