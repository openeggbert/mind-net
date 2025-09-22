//
// Created by robertvokac on 9/2/25.
//
#ifndef REFERENCEVALIDATOR_H
#define REFERENCEVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Reference;

    class ReferenceValidator : public api::ValidatorBase<ReferenceValidator, Reference>
    {
    public:
        ReferenceValidator() = default;
        ~ReferenceValidator() = default; // explicitly make it destructible
        using Model = Reference;

        create_method_prototypes_for_ValidatorBase(Reference)
    };
}
#endif // REFERENCEVALIDATOR_H
