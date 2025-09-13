//
// Created by robertvokac on 9/2/25.
//
#ifndef SM2STATEVALIDATOR_H
#define SM2STATEVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::test::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::test::models::SM2State;

    class SM2StateValidator : public persistence::api::ValidatorBase<SM2StateValidator, SM2State>
    {
    public:
        SM2StateValidator() = default;
        ~SM2StateValidator() = default; // explicitly make it destructible
        using Model = SM2State;

        create_method_prototypes_for_ValidatorBase(SM2State)
    };
}
#endif // SM2STATEVALIDATOR_H
