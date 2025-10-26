//
// Created by robertvokac on 9/2/25.
//
#ifndef R18PREDICTION_LOG_VALIDATOR_H
#define R18PREDICTION_LOG_VALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/R18PredictionLog.h"

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
#endif // R18PREDICTION_LOG_VALIDATOR_H
