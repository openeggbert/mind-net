//
// Created by robertvokac on 9/2/25.
//
#ifndef R2REVIEWVALIDATOR_H
#define R2REVIEWVALIDATOR_H


#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/R2Review.h"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R2Review;

    class R2ReviewValidator : public api::ValidatorBase<R2ReviewValidator, R2Review>
    {
    public:
        R2ReviewValidator() = default;
        ~R2ReviewValidator() = default; // explicitly make it destructible
        using Model = R2Review;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // R2REVIEWVALIDATOR_H
