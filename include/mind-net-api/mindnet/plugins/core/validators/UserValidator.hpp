//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"

namespace mindnet::plugins::core::validators
{
    using db_ptr = mindnet::api::IPersistence*;
    using api::RequestContext;
    using mindnet::plugins::core::models::User;

    class UserValidator : public api::ValidatorBase<UserValidator, User>
    {
    public:
        UserValidator() = default;
        ~UserValidator() = default; // explicitly make it destructible

        using Model = User;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}

