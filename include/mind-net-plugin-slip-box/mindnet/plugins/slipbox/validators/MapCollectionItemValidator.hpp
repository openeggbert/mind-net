//
// Created by robertvokac on 9/2/25.
//
#ifndef MAPCOLLECTINITEMVALIDATOR_H
#define MAPCOLLECTINITEMVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/MapCollectionItem.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::MapCollectionItem;

    class MapCollectionItemValidator : public api::ValidatorBase<MapCollectionItemValidator, MapCollectionItem>
    {
    public:
        MapCollectionItemValidator() = default;
        ~MapCollectionItemValidator() = default; // explicitly make it destructible
        using Model = MapCollectionItem;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}

#endif // MAPCOLLECTINITEMVALIDATOR_H
