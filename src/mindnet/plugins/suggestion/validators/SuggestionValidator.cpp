//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/suggestion/validators/SuggestionValidator.h"

#include "../../../../../include/mindnet/core/Global.h"
#include "mindnet/plugins/suggestion/models/Suggestion.h"
#include "mindnet/api/Persistence.h"

#define Model Suggestion
#define MODEL SUGGESTION
#define model suggestion

namespace mindnet::plugins::suggestion::validators
{
    using validators::SuggestionValidator;
    using mindnet::OperationResult;using mindnet::core::g_configuration;
    OperationResult SuggestionValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SuggestionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult SuggestionValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.from_user_id != ctx.token.user_id,
                  403, "You can create suggestion only for your user.")
        return_if(ctx.db->has_model_with_name(entity.table_name),
                  403, "There is no such model - value for table_name is invalid")
        return_if(entity.review_count != 0,
                  403, "Review count must be 0 during suggestion creation.")
        return_if(
            entity.status != plugins::suggestion::enums::SuggestionStatus::Pending && entity.status != plugins::
            suggestion::enums::SuggestionStatus::Draft,
            403, "Status must be PENDING or DRAFT during suggestion creation.")

        return ok_result;
    }

    OperationResult SuggestionValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.from_user_id != ctx.token.user_id && ctx.role < plugins::core::enums::UserRole::Reviewer,
                  403, "You can not read this suggestion.");

        return ok_result;
    }

    OperationResult SuggestionValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                              const Model& new_entity) const
    {
        return_if(ctx.token.user_id != new_entity.from_user_id,
                  403, "Only author of this suggestion can update it.");
        return_if(old_entity.status == plugins::suggestion::enums::SuggestionStatus::Approved,
                  400, "Suggestion is approved and cannot be updated");

        return ok_result;
    }

    OperationResult SuggestionValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Deleting suggestions is forbidden. Set the suggestion status to cancelled."};
    }

    OperationResult SuggestionValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        if (ctx.role >= plugins::core::enums::UserRole::Reviewer)
        {
            return ok_result;
        }

        mandatory_filter(from_user_id)

        return_if(filter.at("from_user_id") != std::to_string(ctx.token.user_id),
                  403, "You can only list suggestions for your own user.");

        return ok_result;
    }

    string SuggestionValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
