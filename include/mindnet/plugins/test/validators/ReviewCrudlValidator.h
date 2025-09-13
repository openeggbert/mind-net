//
// Created by robertvokac on 9/2/25.
//
#ifndef REVIEWCRUDLVALIDATOR_H
#define REVIEWCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::test::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::test::models::Review;

    class ReviewCrudlValidator : public persistence::api::CrudlValidatorBase<ReviewCrudlValidator, Review>
    {
    public:
        ReviewCrudlValidator() = default;
        ~ReviewCrudlValidator() = default; // explicitly make it destructible
        using Model = Review;

        create_method_prototypes_for_CrudlValidatorBase(Review)
    };
}
#endif // REVIEWCRUDLVALIDATOR_H
