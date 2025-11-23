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


#include "mindnet/plugins/core/validators/HistoryValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/History.hpp"
#include "mindnet/api/Persistence.hpp"


#define Model History
#define MODEL HISTORY
#define model history

namespace mindnet::plugins::core::validators
{
    using validators::HistoryValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult HistoryValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return_if(entity.user_id != ctx.token.user_id,
                  403, "You can only create history for your own user.");

        return ok_result;
    }

    OperationResult HistoryValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.user_id != ctx.token.user_id && ctx.role != mindnet::essential::UserRole::Admin,
                  403, "You can only read history for your own user.");

        return ok_result;
    }

    OperationResult HistoryValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult HistoryValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult HistoryValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        if (ctx.role != mindnet::essential::UserRole::Admin) return ok_result;

        mandatory_filter(user_id)

        return_if(filter.at("user_id") != std::to_string(ctx.token.user_id),
                  403, "You can only list history for your own user.");

        return ok_result;
    }


    OperationResult HistoryValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult HistoryValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult HistoryValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return {405, "History cannot be updated."};
    }

    OperationResult HistoryValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "History cannot be deleted."};
    }

    OperationResult HistoryValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string HistoryValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
