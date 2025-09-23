//
// Created by robertvokac on 9/2/25.
//
#ifndef CONCEPTVALIDATOR_H
#define CONCEPTVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Concept.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Concept;

    class ConceptValidator : public api::ValidatorBase<ConceptValidator, Concept>
    {
    public:
        ConceptValidator() = default;
        ~ConceptValidator() = default; // explicitly make it destructible
        using Model = Concept;

        create_method_prototypes_for_ValidatorBase(Concept)
    };
}
#endif // CONCEPTVALIDATOR_H
