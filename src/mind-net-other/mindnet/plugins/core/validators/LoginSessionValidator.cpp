//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/LoginSessionValidator.h"

#include <regex>
#include "mindnet/core/Configuration.h"
#include "mindnet/core/Global.h"
#include "mindnet/plugins/core/models/LoginSession.h"
#include "mindnet/api/Persistence.h"

#define Model LoginSession
#define MODEL LOGIN_SESSION
#define model login_session

namespace mindnet::plugins::core::validators
{
    using validators::LoginSessionValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;

    OperationResult LoginSessionValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_read_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                       const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_create_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                     const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LoginSessionValidator::validate_list_integrity(const RequestContext& ctx,
                                                                   const string_map& filter) const
    {
        return ok_result;
    }

    string LoginSessionValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
