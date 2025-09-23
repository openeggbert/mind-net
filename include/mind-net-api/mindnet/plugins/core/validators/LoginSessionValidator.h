//
// Created by robertvokac on 9/2/25.
//
#ifndef LOGINSESSIONVALIDATOR_H
#define LOGINSESSIONVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/core/models/LoginSession.h"

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

        create_method_prototypes_for_ValidatorBase(LoginSession)
    };
}
#endif // LOGINSESSIONVALIDATOR_H
