//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/R18State.hpp"

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
