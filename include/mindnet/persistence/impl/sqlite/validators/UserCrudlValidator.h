//
// Created by robertvokac on 9/2/25.
//
#ifndef USERCRUDLVALIDATOR_H
#define USERCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidator.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class UserCrudlValidator : public api::CrudlValidator
    {
    public:
        UserCrudlValidator() = default;
        ~UserCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()


    };

}

#endif // USERCRUDLVALIDATOR_H
