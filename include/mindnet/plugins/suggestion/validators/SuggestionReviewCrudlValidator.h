//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONREVIEWCRUDLVALIDATOR_H
#define SUGGESTIONREVIEWCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::suggestion::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::suggestion::models::SuggestionReview;

    class SuggestionReviewCrudlValidator : public persistence::api::CrudlValidatorBase<SuggestionReviewCrudlValidator, SuggestionReview>
    {
    public:
        SuggestionReviewCrudlValidator() = default;
        ~SuggestionReviewCrudlValidator() = default; // explicitly make it destructible
        using Model = SuggestionReview;

        create_method_prototypes_for_CrudlValidatorBase(SuggestionReview)

    };

}
#endif // SUGGESTIONREVIEWCRUDLVALIDATOR_H
