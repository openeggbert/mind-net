//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONITEMVALIDATOR_H
#define COLLECTIONITEMVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/CollectionItem.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::CollectionItem;

    class CollectionItemValidator : public api::ValidatorBase<
            CollectionItemValidator, CollectionItem>
    {
    public:
        CollectionItemValidator() = default;
        ~CollectionItemValidator() = default; // explicitly make it destructible
        using Model = CollectionItem;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // COLLECTIONITEMVALIDATOR_H
