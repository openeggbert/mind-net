//
// Created by robertvokac on 9/2/25.
//
#ifndef R2STATEVALIDATOR_H
#define R2STATEVALIDATOR_H


#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/R2State.h"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R2State;

    class R2StateValidator : public api::ValidatorBase<R2StateValidator, R2State>
    {
    public:
        R2StateValidator() = default;
        ~R2StateValidator() = default; // explicitly make it destructible
        using Model = R2State;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // R2STATEVALIDATOR_H
