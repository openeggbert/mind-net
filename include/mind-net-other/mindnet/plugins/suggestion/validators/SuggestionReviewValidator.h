//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONREVIEWVALIDATOR_H
#define SUGGESTIONREVIEWVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::suggestion::validators
{
    using api::RequestContext;
    using mindnet::plugins::suggestion::models::SuggestionReview;

    class SuggestionReviewValidator : public api::ValidatorBase<
            SuggestionReviewValidator, SuggestionReview>
    {
    public:
        SuggestionReviewValidator() = default;
        ~SuggestionReviewValidator() = default; // explicitly make it destructible
        using Model = SuggestionReview;

        create_method_prototypes_for_ValidatorBase(SuggestionReview)
    };
}
#endif // SUGGESTIONREVIEWVALIDATOR_H
