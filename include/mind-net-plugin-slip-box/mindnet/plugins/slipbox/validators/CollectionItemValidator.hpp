//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONITEMVALIDATOR_H
#define COLLECTIONITEMVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/CollectionItem.hpp"

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
