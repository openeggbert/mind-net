//
// Created by robertvokac on 9/21/25.
//
#ifndef AUTHLOGVALIDATOR_H
#define AUTHLOGVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/core/models/AuthLog.h"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::AuthLog;

    class AuthLogValidator : public api::ValidatorBase<AuthLogValidator, AuthLog>
    {
    public:
        AuthLogValidator() = default;
        ~AuthLogValidator() = default; // explicitly make it destructible
        using Model = AuthLog;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // AUTHLOGVALIDATOR_H
