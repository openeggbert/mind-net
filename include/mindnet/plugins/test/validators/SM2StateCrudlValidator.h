//
// Created by robertvokac on 9/2/25.
//
#ifndef SM2STATECRUDLVALIDATOR_H
#define SM2STATECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::test::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::test::models::SM2State;

    class SM2StateCrudlValidator : public persistence::api::CrudlValidatorBase<SM2StateCrudlValidator, SM2State>
    {
    public:
        SM2StateCrudlValidator() = default;
        ~SM2StateCrudlValidator() = default; // explicitly make it destructible
        using Model = SM2State;

        create_method_prototypes_for_CrudlValidatorBase(SM2State)
    };
}
#endif // SM2STATECRUDLVALIDATOR_H
