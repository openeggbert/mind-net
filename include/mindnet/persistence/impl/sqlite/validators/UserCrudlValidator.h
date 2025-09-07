//
// Created by robertvokac on 9/2/25.
//
#ifndef USERCRUDLVALIDATOR_H
#define USERCRUDLVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"
#include "mindnet/persistence/api/ICrudlValidator.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::User;

    class UserCrudlValidator : public api::CrudlValidatorBase<UserCrudlValidator, models::User>
    {
    public:
        UserCrudlValidator() = default;
        ~UserCrudlValidator() = default; // explicitly make it destructible

        using Model = User;

        create_method_prototypes_for_CrudlValidatorBase(User)

    };

}

#endif // USERCRUDLVALIDATOR_H
