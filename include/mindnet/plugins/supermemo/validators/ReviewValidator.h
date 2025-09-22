//
// Created by robertvokac on 9/2/25.
//
#ifndef REVIEWVALIDATOR_H
#define REVIEWVALIDATOR_H

#include <memory>

#include "../../../core/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::supermemo::validators
{
    using api::RequestContext;
    using mindnet::plugins::supermemo::models::Review;

    class ReviewValidator : public api::ValidatorBase<ReviewValidator, Review>
    {
    public:
        ReviewValidator() = default;
        ~ReviewValidator() = default; // explicitly make it destructible
        using Model = Review;

        create_method_prototypes_for_ValidatorBase(Review)
    };
}
#endif // REVIEWVALIDATOR_H
