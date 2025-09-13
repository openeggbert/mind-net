//
// Created by robertvokac on 9/2/25.
//
#ifndef REFERENCEVALIDATOR_H
#define REFERENCEVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Reference;

    class ReferenceValidator : public persistence::api::ValidatorBase<ReferenceValidator, Reference>
    {
    public:
        ReferenceValidator() = default;
        ~ReferenceValidator() = default; // explicitly make it destructible
        using Model = Reference;

        create_method_prototypes_for_ValidatorBase(Reference)
    };
}
#endif // REFERENCEVALIDATOR_H
