//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/RefreshTokenValidator.h"

#include <regex>
#include "../../../../../include/mindnet/core/Configuration.h"
#include "../../../../../include/mindnet/core/Global.h"
#include "mindnet/plugins/core/models/RefreshToken.h"
#include "mindnet/api/Persistence.h"

#define Model RefreshToken
#define MODEL REFRESH_TOKEN
#define model refresh_token

namespace mindnet::plugins::core::validators
{
    using validators::RefreshTokenValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;

    OperationResult RefreshTokenValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_read_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_list_authorization(const RequestContext& ctx,
                                                                       const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_create_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                     const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RefreshTokenValidator::validate_list_integrity(const RequestContext& ctx,
                                                                   const string_map& filter) const
    {
        return ok_result;
    }

    string RefreshTokenValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
