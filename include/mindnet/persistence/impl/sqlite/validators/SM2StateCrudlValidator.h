//
// Created by robertvokac on 9/2/25.
//
#ifndef SM2STATECRUDLVALIDATOR_H
#define SM2STATECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::SM2State;

    class SM2StateCrudlValidator : public api::CrudlValidatorBase<SM2StateCrudlValidator, models::SM2State>
    {
    public:
        SM2StateCrudlValidator() = default;
        ~SM2StateCrudlValidator() = default; // explicitly make it destructible
        using Model = SM2State;

        create_method_prototypes_for_CrudlValidatorBase(SM2State)

    };

}
#endif // SM2STATECRUDLVALIDATOR_H
