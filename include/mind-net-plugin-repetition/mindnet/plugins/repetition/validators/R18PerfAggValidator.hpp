//
// Created by robertvokac on 9/2/25.
//
#ifndef R18PERFAGGVALIDATOR_H
#define R18PERFAGGVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/R18PerfAgg.hpp"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R18PerfAgg;

    class R18PerfAggValidator : public api::ValidatorBase<R18PerfAggValidator, R18PerfAgg>
    {
    public:
        R18PerfAggValidator() = default;
        ~R18PerfAggValidator() = default; // explicitly make it destructible
        using Model = R18PerfAgg;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // R18PERFAGGVALIDATOR_H
