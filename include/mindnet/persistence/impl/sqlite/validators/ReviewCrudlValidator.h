//
// Created by robertvokac on 9/2/25.
//
#ifndef REVIEWCRUDLVALIDATOR_H
#define REVIEWCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

//namespace mindnet::persistence
//{
//    class Persistence;
//}

namespace mindnet::persistence::impl::sqlite::validators
{
    //using db_ = mindnet::persistence::Persistence*;
    using api::RequestContext;
    using models::Review;

    class ReviewCrudlValidator : public api::CrudlValidatorBase<ReviewCrudlValidator, models::Review>
    {
    public:
        ReviewCrudlValidator() = default;
        ~ReviewCrudlValidator() = default; // explicitly make it destructible
        using Model = Review;

        create_method_prototypes_for_CrudlValidatorBase(Review)

    };

}
#endif // REVIEWCRUDLVALIDATOR_H
