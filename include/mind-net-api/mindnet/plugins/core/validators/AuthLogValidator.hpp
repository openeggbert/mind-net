//
// Created by robertvokac on 9/21/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/AuthLog.hpp"

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
