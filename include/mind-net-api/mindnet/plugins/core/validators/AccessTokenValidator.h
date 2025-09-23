//
// Created by robertvokac on 9/2/25.
//
#ifndef ACCESSTOKENVALIDATOR_H
#define ACCESSTOKENVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/core/models/AccessToken.h"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::AccessToken;

    class AccessTokenValidator : public api::ValidatorBase<AccessTokenValidator, AccessToken>
    {
    public:
        AccessTokenValidator() = default;
        ~AccessTokenValidator() = default; // explicitly make it destructible
        using Model = AccessToken;

        create_method_prototypes_for_ValidatorBase(AccessToken)
    };
}
#endif // ACCESSTOKENVALIDATOR_H
