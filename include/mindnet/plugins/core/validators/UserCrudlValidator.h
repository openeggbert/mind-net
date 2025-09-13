//
// Created by robertvokac on 9/2/25.
//
#ifndef USERCRUDLVALIDATOR_H
#define USERCRUDLVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::plugins::core::validators
{
    using db_ptr = mindnet::persistence::api::IPersistence*;
    using persistence::api::RequestContext;
    using mindnet::plugins::core::models::User;

    class UserCrudlValidator : public persistence::api::CrudlValidatorBase<UserCrudlValidator, User>
    {
    public:
        UserCrudlValidator() = default;
        ~UserCrudlValidator() = default; // explicitly make it destructible

        using Model = User;

        create_method_prototypes_for_CrudlValidatorBase(User)
    };
}

#endif // USERCRUDLVALIDATOR_H
