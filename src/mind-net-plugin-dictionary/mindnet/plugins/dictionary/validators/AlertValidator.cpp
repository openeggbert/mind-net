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

#include "mindnet/plugins/dictionary/validators/AlertValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/dictionary/models/Alert.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model Alert
#define MODEL ALERT
#define model alert

namespace mindnet::plugins::dictionary::validators
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