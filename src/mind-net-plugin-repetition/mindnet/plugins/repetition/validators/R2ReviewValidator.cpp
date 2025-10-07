//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/R2ReviewValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/R2Review.h"
#include "mindnet/api/Persistence.h"

#define Model R2Review
#define MODEL R2_REVIEW
#define model r2_review

namespace mindnet::plugins::repetition::validators
{
    using validators::R2ReviewValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;
    OperationResult R2ReviewValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R2ReviewValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R2ReviewValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult R2ReviewValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R2ReviewValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult R2ReviewValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2ReviewValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2ReviewValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                          const Model& new_entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2ReviewValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2ReviewValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Unsupported operation."};
    }

    string R2ReviewValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
