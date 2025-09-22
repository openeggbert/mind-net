//
// Created by robertvokac on 9/2/25.
//
#ifndef USERVALIDATOR_H
#define USERVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"

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

        create_method_prototypes_for_ValidatorBase(User)
    };
}

#endif // USERVALIDATOR_H
