//
// Created by robertvokac on 9/2/25.
//
#ifndef REFRESHTOKENVALIDATOR_H
#define REFRESHTOKENVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"
#include "mindnet/plugins/core/models/RefreshToken.h"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::RefreshToken;

    class RefreshTokenValidator : public api::ValidatorBase<RefreshTokenValidator, RefreshToken>
    {
    public:
        RefreshTokenValidator() = default;
        ~RefreshTokenValidator() = default; // explicitly make it destructible
        using Model = RefreshToken;

        create_method_prototypes_for_ValidatorBase(RefreshToken)
    };
}
#endif // REFRESHTOKENVALIDATOR_H
