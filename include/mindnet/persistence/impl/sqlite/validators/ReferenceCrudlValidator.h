//
// Created by robertvokac on 9/2/25.
//
#ifndef REFERENCECRUDLVALIDATOR_H
#define REFERENCECRUDLVALIDATOR_H


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
    using models::Reference;

    class ReferenceCrudlValidator : public api::CrudlValidatorBase<ReferenceCrudlValidator, models::Reference>
    {
    public:
        ReferenceCrudlValidator() = default;
        ~ReferenceCrudlValidator() = default; // explicitly make it destructible
        using Model = Reference;

        create_method_prototypes_for_CrudlValidatorBase(Reference)

    };

}
#endif // REFERENCECRUDLVALIDATOR_H
