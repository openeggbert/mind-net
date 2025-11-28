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

#include "mindnet/plugins/core/validators/TeamValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/Team.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model Team
#define MODEL TEAM
#define model team

namespace mindnet::plugins::core::validators
{
    using validators::TeamValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult TeamValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role<mindnet::essential::UserRole::Editor,
                           403, "User does not have permission to create a team.");

        return ok_result;
    }

    OperationResult TeamValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        return_if(ctx.role != mindnet::essential::UserRole::Admin && ctx.token.user_id != new_entity.leader_id,
                  403, "Only team leader can update the team.")

        return_if(old_entity.leader_id != new_entity.leader_id && ctx.role<mindnet::essential::UserRole::Admin,
                                                                           400,
                                                                           "leader_id cannot be changed by yourself. Contact admin."
        )

        return ok_result;
    }

    OperationResult TeamValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role<mindnet::essential::UserRole::Admin,
                           403, "Only admins can delete a team. Contact admin");

        return ok_result;
    }

    OperationResult TeamValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.created_by != ctx.token.user_id,
                  400, "created_by must be set to the logged in user.")
        return_if(entity.leader_id != ctx.token.user_id,
                  400, "leader_id must be set to the logged in user.")

        return ok_result;
    }

    OperationResult TeamValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TeamValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string TeamValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model