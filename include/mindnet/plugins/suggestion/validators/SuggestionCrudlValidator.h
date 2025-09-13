//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONCRUDLVALIDATOR_H
#define SUGGESTIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::suggestion::validators

{
    using persistence::api::RequestContext;
    using mindnet::plugins::suggestion::models::Suggestion;

    class SuggestionCrudlValidator : public persistence::api::CrudlValidatorBase<SuggestionCrudlValidator, Suggestion>
    {
    public:
        SuggestionCrudlValidator() = default;
        ~SuggestionCrudlValidator() = default; // explicitly make it destructible
        using Model = Suggestion;

        create_method_prototypes_for_CrudlValidatorBase(Suggestion)
    };
}
#endif // SUGGESTIONCRUDLVALIDATOR_H
