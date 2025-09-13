//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/SuggestionReviewCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/suggestion/models/SuggestionReview.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model SuggestionReview
#define MODEL SUGGESTION_REVIEW
#define model suggestion_review

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::SuggestionReviewCrudlValidator;
    using api::OperationResult;

    OperationResult SuggestionReviewCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {


        return_if (ctx.role < plugins::core::enums::UserRole::REVIEWER,
            403, "You can not create suggestion reviews.");

        return ok_result;
    }

    OperationResult SuggestionReviewCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {


        auto suggestion = api::find_suggestion(ctx, entity.suggestion_id);
        check_found(suggestion);

        return_if (ctx.role < plugins::core::enums::UserRole::REVIEWER && suggestion.first.from_user_id != ctx.token.user_id,
            403, "You can not read this suggestion.");

        return ok_result;
    }

    OperationResult SuggestionReviewCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return_if (ctx.token.user_id != old_entity.reviewer_id,
            403, "Only creator of this suggestion review can update it");
        return_if (old_entity.decision_status != plugins::suggestion::enums::DecisionStatus::REQUESTS_FEEDBACK,
            400, "Only suggestion reviews with status REQUESTS_FEEDBACK can be updated.");

        return ok_result;
    }

    OperationResult SuggestionReviewCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {


        return {405, "Deleting suggestions is forbidden. "};
    }

    OperationResult SuggestionReviewCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {


        if (ctx.role >= plugins::core::enums::UserRole::REVIEWER) return ok_result;
        return {403, "You can not list suggestion reviews."};

        return ok_result;
    }

    string SuggestionReviewCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model