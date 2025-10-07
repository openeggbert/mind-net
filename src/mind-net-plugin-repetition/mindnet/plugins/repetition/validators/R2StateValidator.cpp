//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/R2StateValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/R2State.h"
#include "mindnet/api/Persistence.h"

#define Model R2State
#define MODEL R2_STATE
#define model r2_state

namespace mindnet::plugins::repetition::validators
{
    using validators::R2StateValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;
    OperationResult R2StateValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R2StateValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R2StateValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult R2StateValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R2StateValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult R2StateValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2StateValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2StateValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2StateValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R2StateValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Unsupported operation."};
    }

    string R2StateValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
