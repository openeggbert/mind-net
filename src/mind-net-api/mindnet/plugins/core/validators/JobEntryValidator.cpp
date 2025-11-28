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

#include "mindnet/plugins/core/validators/JobEntryValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/JobEntry.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model JobEntry
#define MODEL JOB_ENTRY
#define model job_entry

namespace mindnet::plugins::core::validators
{
    using validators::JobEntryValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult JobEntryValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        if (ctx.role < mindnet::essential::UserRole::Admin) return status_403_forbidden;

        //admin or super admin
        if (ctx.role < mindnet::essential::UserRole::System)
        {
            if (old_entity.equals_or_differs_only_in_enabled_or_configuration(new_entity)) { return ok_result; }
            return {400, "You can modify only the columns enabled and configuration."};
        }
        // system
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        if (ctx.role < mindnet::essential::UserRole::Admin) return status_403_forbidden;
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        return ok_result;
    }

OperationResult JobEntryValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string JobEntryValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
