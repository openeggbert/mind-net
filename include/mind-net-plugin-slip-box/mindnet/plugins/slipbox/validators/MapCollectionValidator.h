//
// Created by robertvokac on 9/2/25.
//
#ifndef MAPCOLLECTINVALIDATOR_H
#define MAPCOLLECTINVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Map.h"
#include "mindnet/plugins/slipbox/models/MapCollection.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::MapCollection;

    class MapCollectionValidator : public api::ValidatorBase<MapCollectionValidator, MapCollection>
    {
    public:
        MapCollectionValidator() = default;
        ~MapCollectionValidator() = default; // explicitly make it destructible
        using Model = MapCollection;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}

#endif // MAPCOLLECTINVALIDATOR_H
