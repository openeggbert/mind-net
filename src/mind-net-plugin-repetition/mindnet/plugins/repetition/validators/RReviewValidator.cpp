//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/RReviewValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/repetition/models/RReview.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/repetition/models/RSession.hpp"

#define Model RReview
#define MODEL R_REVIEW
#define model r_review

namespace mindnet::plugins::repetition::validators
{
    using validators::RReviewValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

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

        // Users can only list their own sessions
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoi(it->second) != ctx.token.user_id)
        {
            return {403, "Can only list your own r_sessions"};
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
