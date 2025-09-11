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

    OperationResult SuggestionCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {


        return_if (entity.from_user_id != ctx.token.user_id,
            403, "You can create suggestion only for your user.")
        return_if (ctx.db.has_repository_with_name(entity.table_name),
            403, "There is no such model - value for table_name is invalid")
        return_if (entity.review_count != 0,
            403, "Review count must be 0 during suggestion creation.")
        return_if (entity.status != enums::SuggestionStatus::PENDING && entity.status != enums::SuggestionStatus::DRAFT,
            403, "Status must be PENDING or DRAFT during suggestion creation.")

        return ok_result;
    }

    OperationResult SuggestionCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {


        return_if (entity.from_user_id != ctx.token.user_id && ctx.role < enums::UserRole::REVIEWER,
            403, "You can not read this suggestion.");

        return ok_result;
    }

    OperationResult SuggestionCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return_if (ctx.token.user_id != new_entity.from_user_id,
            403, "Only author of this suggestion can update it.");
        return_if (old_entity.status == enums::SuggestionStatus::APPROVED,
            400, "Suggestion is approved and cannot be updated");

        return ok_result;
    }

    OperationResult SuggestionCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {


        return {405, "Deleting suggestions is forbidden. Set the suggestion status to cancelled."};
    }

    OperationResult SuggestionCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {

        if (ctx.role >= enums::UserRole::REVIEWER)
        {
            return ok_result;
        }

        mandatory_filter(from_user_id)

        return_if (filter.at("from_user_id") != std::to_string(ctx.token.user_id),
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