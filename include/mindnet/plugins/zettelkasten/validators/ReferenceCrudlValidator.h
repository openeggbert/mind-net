//
// Created by robertvokac on 9/2/25.
//
#ifndef REFERENCECRUDLVALIDATOR_H
#define REFERENCECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Reference;

    class ReferenceCrudlValidator : public persistence::api::CrudlValidatorBase<ReferenceCrudlValidator, Reference>
    {
    public:
        ReferenceCrudlValidator() = default;
        ~ReferenceCrudlValidator() = default; // explicitly make it destructible
        using Model = Reference;

        create_method_prototypes_for_CrudlValidatorBase(Reference)

    };

}
#endif // REFERENCECRUDLVALIDATOR_H
