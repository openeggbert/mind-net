//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Collection.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Collection;

    class CollectionValidator : public api::ValidatorBase<CollectionValidator, Collection>
    {
    public:
        CollectionValidator() = default;
        ~CollectionValidator() = default; // explicitly make it destructible
        using Model = Collection;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
