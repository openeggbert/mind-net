//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/R0State.hpp"

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
