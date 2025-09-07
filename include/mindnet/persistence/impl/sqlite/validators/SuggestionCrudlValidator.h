//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONCRUDLVALIDATOR_H
#define SUGGESTIONCRUDLVALIDATOR_H


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
    using models::Suggestion;

    class SuggestionCrudlValidator : public api::CrudlValidatorBase<SuggestionCrudlValidator, models::Suggestion>
    {
    public:
        SuggestionCrudlValidator() = default;
        ~SuggestionCrudlValidator() = default; // explicitly make it destructible
        using Model = Suggestion;

        create_method_prototypes_for_CrudlValidatorBase(Suggestion)

    };

}
#endif // SUGGESTIONCRUDLVALIDATOR_H
