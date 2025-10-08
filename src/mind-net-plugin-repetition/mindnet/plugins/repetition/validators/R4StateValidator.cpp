//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/R4StateValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/repetition/models/R4State.h"
#include "mindnet/api/Persistence.h"

#define Model R4State
#define MODEL R4_STATE
#define model r4_state

namespace mindnet::plugins::repetition::validators
{
    using validators::R4StateValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;
    OperationResult R4StateValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R4StateValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R4StateValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult R4StateValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult R4StateValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult R4StateValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R4StateValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R4StateValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R4StateValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult R4StateValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Unsupported operation."};
    }

    string R4StateValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
