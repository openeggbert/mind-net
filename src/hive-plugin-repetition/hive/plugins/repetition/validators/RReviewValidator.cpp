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

#include "hive/plugins/repetition/validators/RReviewValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/core/enums/SingleRight.hpp"
#include "hive/plugins/repetition/models/RReview.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/repetition/models/RSession.hpp"

#define Model RReview
#define MODEL R_REVIEW
#define model r_review

namespace hive::plugins::repetition::validators
{
    using validators::RReviewValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult RReviewValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        // User can only create r_reviews for themselves
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only create r_reviews for yourself"};
        }
        return ok_result;
    }

    OperationResult RReviewValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Users can only read their own r_reviews
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only access your own r_reviews"};
        }
        return ok_result;
    }

    OperationResult RReviewValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult RReviewValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        mandatory_filter(user_id)

        // Users can only list their own r_review
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoll(it->second) != ctx.token.user_id)
        {
            return {403, "Can only list your own r_reviews"};
        }
        return ok_result;
    }

    OperationResult RReviewValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto session_result = ctx.db->read(models::R_SESSION_DEFINITION, ctx.token, entity.r_session_id);
        if (session_result.second.ko())
        {
            return {500, "Loading session " + std::to_string(entity.r_session_id) + " failed."};
        }
        models::RSession session;
        session.from_values(session_result.first);
        if (session.map_id != entity.map_id)
        {
            return {403, "Map ID must be same for both r_session and r_review."};
        }

        return ok_result;
    }

    OperationResult RReviewValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult RReviewValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string RReviewValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model