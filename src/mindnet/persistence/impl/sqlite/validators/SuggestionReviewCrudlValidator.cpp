//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/SuggestionReviewCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/SuggestionReview.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model SuggestionReview
#define MODEL SUGGESTION_REVIEW
#define model suggestion_review

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::SuggestionReviewCrudlValidator;

    operation_result SuggestionReviewCrudlValidator::validate_create(const ValidatorContext& ctx, const Model& entity) const
    {


        return_if (ctx.logged_user.role < enums::UserRole::REVIEWER,
            403, "You can not create suggestion reviews.");

        return ok_result;
    }

    operation_result SuggestionReviewCrudlValidator::validate_read(const ValidatorContext& ctx, const Model& entity) const
    {


        auto suggestion = api::find_suggestion(ctx, entity.suggestion_id);
        check_found(suggestion);

        return_if (ctx.logged_user.role < enums::UserRole::REVIEWER && suggestion.first.from_user_id != ctx.logged_user.get_id(),
            403, "You can not read this suggestion.");

        return ok_result;
    }

    operation_result SuggestionReviewCrudlValidator::validate_update(const ValidatorContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return_if (ctx.logged_user.get_id() != old_entity.reviewer_id,
            403, "Only creator of this suggestion review can update it");
        return_if (old_entity.decision_status != enums::DecisionStatus::REQUESTS_FEEDBACK,
            400, "Only suggestion reviews with status REQUESTS_FEEDBACK can be updated.");

        return ok_result;
    }

    operation_result SuggestionReviewCrudlValidator::validate_delete(const ValidatorContext& ctx, const Model& entity)  const
    {


        return {405, "Deleting suggestions is forbidden. "};
    }

    operation_result SuggestionReviewCrudlValidator::validate_list(const ValidatorContext& ctx, const string_map& filter) const
    {


        if (ctx.logged_user.role >= enums::UserRole::REVIEWER) return ok_result;
        return {403, "You can not list suggestion reviews."};

        return ok_result;
    }

    string SuggestionReviewCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model