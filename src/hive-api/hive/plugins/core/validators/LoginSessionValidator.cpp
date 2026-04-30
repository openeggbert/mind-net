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

#include "hive/plugins/core/validators/LoginSessionValidator.hpp"

#include <regex>
#include "hive/essential/Configuration.hpp"
#include "hive/essential/Global.hpp"
#include "hive/plugins/core/models/LoginSession.hpp"
#include "hive/api/Persistence.hpp"

#define Model LoginSession
#define MODEL LOGIN_SESSION
#define model login_session

namespace hive::plugins::core::validators
{
    using validators::LoginSessionValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult LoginSessionValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_read_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                       const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_create_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                     const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_list_integrity(const RequestContext& ctx,
                                                                   const string_map& filter) const
    {
        return ok_result;
    }

    string LoginSessionValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model