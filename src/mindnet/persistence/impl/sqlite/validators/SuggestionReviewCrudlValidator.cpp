//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/SuggestionReviewCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/SuggestionReview.h"
#include "mindnet/persistence/Persistence.h"

#define Model SuggestionReview
#define MODEL SUGGESTION_REVIEW
#define model suggestion_review

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::SuggestionReviewCrudlValidator;

    operation_result SuggestionReviewCrudlValidator::can_create(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (role < enums::UserRole::REVIEWER,
            403, "You can not create suggestion reviews.");

        return ok_result;
    }

    operation_result SuggestionReviewCrudlValidator::can_read(db_& db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        auto suggestion = api::find_suggestion(db, token, new_entity.suggestion_id);
        check_found(suggestion);

        return_if (role < enums::UserRole::REVIEWER && suggestion.first.from_user_id != logged_in_user.get_id(),
            403, "You can not read this suggestion.");

        return ok_result;
    }

    operation_result SuggestionReviewCrudlValidator::can_update(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        return_if (logged_in_user.get_id() != old_entity.reviewer_id,
            403, "Only creator of this suggestion review can update it");
        return_if (old_entity.decision_status != enums::DecisionStatus::REQUESTS_FEEDBACK,
            400, "Only suggestion reviews with status REQUESTS_FEEDBACK can be updated.");

        return ok_result;
    }

    operation_result SuggestionReviewCrudlValidator::can_delete(db_& db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        return {405, "Deleting suggestions is forbidden. "};
    }

    operation_result SuggestionReviewCrudlValidator::can_list(db_& db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)

        if (role >= enums::UserRole::REVIEWER) return ok_result;
        return {403, "You can not list suggestion reviews."};

        return ok_result;
    }

    string SuggestionReviewCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}
