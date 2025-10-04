//
// Created by robertvokac on 9/2/25.
//
#ifndef TERMVALIDATOR_H
#define TERMVALIDATOR_H


#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Term.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Term;

    class TermValidator : public api::ValidatorBase<TermValidator, Term>
    {
    public:
        TermValidator() = default;
        ~TermValidator() = default; // explicitly make it destructible
        using Model = Term;

        create_method_prototypes_for_ValidatorBase(Term)
    };
}
#endif // TERMVALIDATOR_H
