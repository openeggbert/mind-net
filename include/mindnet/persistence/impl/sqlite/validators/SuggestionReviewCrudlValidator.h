//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONREVIEWCRUDLVALIDATOR_H
#define SUGGESTIONREVIEWCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class SuggestionReviewCrudlValidator : public api::CrudlValidator
    {
    public:
        SuggestionReviewCrudlValidator() = default;
        ~SuggestionReviewCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // SUGGESTIONREVIEWCRUDLVALIDATOR_H
