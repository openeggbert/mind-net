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

#include "mindnet/plugins/core/validators/AuthLogValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/AuthLog.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model AuthLog
#define MODEL AUTH_LOG
#define model auth_log

namespace mindnet::plugins::core::validators
{
    using validators::AuthLogValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    // ===== Authorization =====
    OperationResult AuthLogValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        // Only system/internal processes should create auth logs
        return ok_result;
    }

    OperationResult AuthLogValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.role == mindnet::essential::UserRole::Admin) return ok_result;

        // Users can read only their own auth logs
        return_if(entity.user_id != ctx.token.user_id,
                  403, "You can only read your own auth logs.");

        return ok_result;
    }

    OperationResult AuthLogValidator::validate_update_authorization(const RequestContext& ctx,
                                                                    const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return {405, "Auth log entries cannot be updated."};
    }

    OperationResult AuthLogValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return {405, "Auth log entries cannot be deleted."};
    }

    OperationResult AuthLogValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        if (ctx.role == mindnet::essential::UserRole::Admin) return ok_result;

        // normal users may only list their own logs
        mandatory_filter(user_id)

        return_if(filter.at("user_id") != std::to_string(ctx.token.user_id),
                  403, "You can only list your own auth logs.");

        return ok_result;
    }

    // ===== Integrity =====
    OperationResult AuthLogValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // Validate required fields
        return_if(entity.endpoint.empty(), 400, "Endpoint is required");
        return_if(entity.status_code == 0, 400, "Status code is required");

        return ok_result;
    }

    OperationResult AuthLogValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // Authorization already checked user_id vs ctx.token.user_id
        return ok_result;
    }

    OperationResult AuthLogValidator::validate_update_integrity(const RequestContext& ctx,
                                                                const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return {405, "Auth log entries cannot be updated."};
    }

    OperationResult AuthLogValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Auth log entries cannot be deleted."};
    }

    OperationResult AuthLogValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        // Authorization already enforces user_id filter
        return ok_result;
    }

    string AuthLogValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
