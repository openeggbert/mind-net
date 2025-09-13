//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONVALIDATOR_H
#define COLLECTIONVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Collection;

    class CollectionValidator : public api::ValidatorBase<CollectionValidator, Collection>
    {
    public:
        CollectionValidator() = default;
        ~CollectionValidator() = default; // explicitly make it destructible
        using Model = Collection;

        create_method_prototypes_for_ValidatorBase(Collection)
    };
}
#endif // COLLECTIONVALIDATOR_H
