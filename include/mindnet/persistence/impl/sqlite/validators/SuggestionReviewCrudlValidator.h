//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONREVIEWCRUDLVALIDATOR_H
#define SUGGESTIONREVIEWCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::SuggestionReview;

    class SuggestionReviewCrudlValidator : public api::CrudlValidatorBase<SuggestionReviewCrudlValidator, models::SuggestionReview>
    {
    public:
        SuggestionReviewCrudlValidator() = default;
        ~SuggestionReviewCrudlValidator() = default; // explicitly make it destructible
        using Model = SuggestionReview;

        create_method_prototypes_for_CrudlValidatorBase(SuggestionReview)

    };

}
#endif // SUGGESTIONREVIEWCRUDLVALIDATOR_H
