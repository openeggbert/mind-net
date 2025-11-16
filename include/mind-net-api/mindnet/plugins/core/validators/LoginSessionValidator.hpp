//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/LoginSession.hpp"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::LoginSession;

    class LoginSessionValidator : public api::ValidatorBase<LoginSessionValidator, LoginSession>
    {
    public:
        LoginSessionValidator() = default;
        ~LoginSessionValidator() = default; // explicitly make it destructible
        using Model = LoginSession;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
