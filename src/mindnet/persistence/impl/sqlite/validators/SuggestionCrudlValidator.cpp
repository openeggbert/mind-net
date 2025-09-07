//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/SuggestionCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Suggestion.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Suggestion
#define MODEL SUGGESTION
#define model suggestion

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::SuggestionCrudlValidator;

    operation_result SuggestionCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (new_entity.from_user_id != logged_in_user.get_id(),
            403, "You can create suggestion only for your user.")
        return_if (db->has_repository(new_entity.table_name),
            403, "There is no such model - value for table_name is invalid")
        return_if (new_entity.review_count != 0,
            403, "Review count must be 0 during suggestion creation.")
        return_if (new_entity.status != enums::SuggestionStatus::PENDING && new_entity.status != enums::SuggestionStatus::DRAFT,
            403, "Status must be PENDING or DRAFT during suggestion creation.")

        return ok_result;
    }

    operation_result SuggestionCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        return_if (entity.from_user_id != logged_in_user.get_id() && role < enums::UserRole::REVIEWER,
            403, "You can not read this suggestion.");

        return ok_result;
    }

    operation_result SuggestionCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        return_if (logged_in_user.get_id() != new_entity.from_user_id,
            403, "Only author of this suggestion can update it.");
        return_if (old_entity.status == enums::SuggestionStatus::APPROVED,
            400, "Suggestion is approved and cannot be updated");

        return ok_result;
    }

    operation_result SuggestionCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        return {405, "Deleting suggestions is forbidden. Set the suggestion status to cancelled."};
    }

    operation_result SuggestionCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)
        if (role >= enums::UserRole::REVIEWER)
        {
            return ok_result;
        }

        mandatory_filter(from_user_id)

        return_if (filter["from_user_id"] != std::to_string(logged_in_user.get_id()),
            403, "You can only list suggestions for your own user.");

        return ok_result;
    }

    string SuggestionCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model