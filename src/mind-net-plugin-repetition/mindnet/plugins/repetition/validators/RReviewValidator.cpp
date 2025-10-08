//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/RReviewValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/RReview.h"
#include "mindnet/api/Persistence.h"

#define Model RReview
#define MODEL R_REVIEW
#define model r_review

namespace mindnet::plugins::repetition::validators
{
    using validators::RReviewValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;
    OperationResult RReviewValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RReviewValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult RReviewValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult RReviewValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult RReviewValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                          const Model& new_entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult RReviewValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult RReviewValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Unsupported operation."};
    }

    string RReviewValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
