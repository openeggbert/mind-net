//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/R18PredictionLog.hpp"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R18PredictionLog;

    class R18PredictionLogValidator : public api::ValidatorBase<R18PredictionLogValidator, R18PredictionLog>
    {
    public:
        R18PredictionLogValidator() = default;
        ~R18PredictionLogValidator() = default; // explicitly make it destructible
        using Model = R18PredictionLog;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
