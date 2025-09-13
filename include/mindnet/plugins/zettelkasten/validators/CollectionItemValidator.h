//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONITEMVALIDATOR_H
#define COLLECTIONITEMVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::CollectionItem;

    class CollectionItemValidator : public persistence::api::ValidatorBase<
            CollectionItemValidator, CollectionItem>
    {
    public:
        CollectionItemValidator() = default;
        ~CollectionItemValidator() = default; // explicitly make it destructible
        using Model = CollectionItem;

        create_method_prototypes_for_ValidatorBase(CollectionItem)
    };
}
#endif // COLLECTIONITEMVALIDATOR_H
