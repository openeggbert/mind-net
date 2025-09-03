//
// Created by robertvokac on 9/2/25.
//
#ifndef SUGGESTIONCRUDLVALIDATOR_H
#define SUGGESTIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class SuggestionCrudlValidator : public api::CrudlValidator
    {
    public:
        SuggestionCrudlValidator() = default;
        ~SuggestionCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // SUGGESTIONCRUDLVALIDATOR_H
