//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONVALIDATOR_H
#define SUGGESTIONVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::suggestion::validators

{
    using persistence::api::RequestContext;
    using mindnet::plugins::suggestion::models::Suggestion;

    class SuggestionValidator : public persistence::api::ValidatorBase<SuggestionValidator, Suggestion>
    {
    public:
        SuggestionValidator() = default;
        ~SuggestionValidator() = default; // explicitly make it destructible
        using Model = Suggestion;

        create_method_prototypes_for_ValidatorBase(Suggestion)
    };
}
#endif // SUGGESTIONVALIDATOR_H
