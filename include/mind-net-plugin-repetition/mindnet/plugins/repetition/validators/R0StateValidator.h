//
// Created by robertvokac on 9/2/25.
//
#ifndef R0STATEVALIDATOR_H
#define R0STATEVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/R0State.h"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R0State;

    class R0StateValidator : public api::ValidatorBase<R0StateValidator, R0State>
    {
    public:
        R0StateValidator() = default;
        ~R0StateValidator() = default; // explicitly make it destructible
        using Model = R0State;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // R0STATEVALIDATOR_H
