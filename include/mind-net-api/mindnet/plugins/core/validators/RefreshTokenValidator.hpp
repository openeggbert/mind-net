//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/RefreshToken.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
