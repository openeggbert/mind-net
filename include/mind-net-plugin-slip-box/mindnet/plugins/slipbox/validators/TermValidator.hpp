//
// Created by robertvokac on 9/2/25.
//
#ifndef TERMVALIDATOR_H
#define TERMVALIDATOR_H


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Term.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // TERMVALIDATOR_H
