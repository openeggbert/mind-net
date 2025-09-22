//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/suggestion/validators/SuggestionReviewValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/suggestion/models/SuggestionReview.h"
#include "mindnet/api/Persistence.h"

#define Model SuggestionReview
#define MODEL SUGGESTION_REVIEW
#define model suggestion_review

namespace mindnet::plugins::suggestion::validators
{
    using validators::SuggestionReviewValidator;
    using mindnet::OperationResult;using mindnet::core::g_configuration;
    OperationResult SuggestionReviewValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult SuggestionReviewValidator::validate_create_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return_if(ctx.role < plugins::core::enums::UserRole::Reviewer,
                  403, "You can not create suggestion reviews.");

        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto suggestion = find_suggestion(ctx, entity.suggestion_id);
        check_found(suggestion);

        return_if(
            ctx.role < plugins::core::enums::UserRole::Reviewer && suggestion.first.from_user_id != ctx.token.user_id,
            403, "You can not read this suggestion.");

        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return_if(ctx.token.user_id != old_entity.reviewer_id,
                  403, "Only creator of this suggestion review can update it");
        return_if(old_entity.decision_status != plugins::suggestion::enums::DecisionStatus::RequestsFeedback,
                  400, "Only suggestion reviews with status REQUESTS_FEEDBACK can be updated.");

        return ok_result;
    }

    OperationResult SuggestionReviewValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return {405, "Deleting suggestions is forbidden. "};
    }

    OperationResult SuggestionReviewValidator::validate_list_integrity(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        if (ctx.role >= plugins::core::enums::UserRole::Reviewer) return ok_result;
        return {403, "You can not list suggestion reviews."};

        return ok_result;
    }

    string SuggestionReviewValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
