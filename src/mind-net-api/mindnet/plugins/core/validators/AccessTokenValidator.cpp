//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/AccessTokenValidator.hpp"

#include <regex>
#include "mindnet/essential/Configuration.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/AccessToken.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model AccessToken
#define MODEL ACCESS_TOKEN
#define model access_token

namespace mindnet::plugins::core::validators
{
    using validators::AccessTokenValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult AccessTokenValidator::validate_create_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_read_authorization(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_update_authorization(const RequestContext& ctx,
                                                                        const Model& old_entity,
                                                                        const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_list_authorization(const RequestContext& ctx,
                                                                      const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_create_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AccessTokenValidator::validate_list_integrity(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        return ok_result;
    }

    string AccessTokenValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
