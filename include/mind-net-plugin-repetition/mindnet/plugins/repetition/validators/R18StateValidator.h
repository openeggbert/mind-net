//
// Created by robertvokac on 9/2/25.
//
#ifndef R18STATEVALIDATOR_H
#define R18STATEVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/R18State.h"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R18State;

    class R18StateValidator : public api::ValidatorBase<R18StateValidator, R18State>
    {
    public:
        R18StateValidator() = default;
        ~R18StateValidator() = default; // explicitly make it destructible
        using Model = R18State;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // R18STATEVALIDATOR_H
