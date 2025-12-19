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

#include "mindnet/plugins/core/validators/ApiLogValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/ApiLog.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model ApiLog
#define MODEL API_LOG
#define model api_log

namespace mindnet::plugins::core::validators
{
    using validators::ApiLogValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    // ===== Authorization =====
    OperationResult ApiLogValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ApiLogValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.role >= mindnet::essential::UserRole::Admin) return ok_result;

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
        if (ctx.role == mindnet::essential::UserRole::Admin) return ok_result;

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