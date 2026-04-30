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

#include "hive/plugins/core/validators/ErrorValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/core/models/Error.hpp"
#include "hive/api/Persistence.hpp"

#define Model Error
#define MODEL ERROR
#define model error

namespace hive::plugins::core::validators
{
    using validators::ErrorValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    // ===== Authorization =====
    OperationResult ErrorValidator::validate_create_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        if (ctx.role < essential::UserRole::System)
            return {403, "Only system can create errors."};

        return ok_result;
    }

    OperationResult ErrorValidator::validate_read_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        // Only admins can read error
        return_if(ctx.role < hive::essential::UserRole::Admin,
                  403, "Only administrators can access errors.");

        return ok_result;
    }

    OperationResult ErrorValidator::validate_update_authorization(const RequestContext& ctx,
                                                                          const Model& old_entity,
                                                                          const Model& new_entity) const
    {
        // Only admins can update error
        return_if(ctx.role < hive::essential::UserRole::Admin,
                  403, "Only administrators can update errors.");
        return ok_result;
    }

    OperationResult ErrorValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        return {405, "Error entries cannot be deleted."};
    }

    OperationResult ErrorValidator::validate_list_authorization(const RequestContext& ctx,
                                                                        const string_map& filter) const
    {
        // Only admins can list error
        return_if(ctx.role < hive::essential::UserRole::Admin,
                  403, "Only administrators can list errors.");

        return ok_result;
    }

    // ===== Integrity =====
    OperationResult ErrorValidator::validate_create_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ErrorValidator::validate_read_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ErrorValidator::validate_update_integrity(const RequestContext& ctx,
                                                                      const Model& old_entity,
                                                                      const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult ErrorValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return {405, "Error entries cannot be deleted."};
    }

    OperationResult ErrorValidator::validate_list_integrity(const RequestContext& ctx,
                                                                    const string_map& filter) const
    {
        return ok_result;
    }

    string ErrorValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model