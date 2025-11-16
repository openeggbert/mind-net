//
// Created by robertvokac on 9/2/25.
//
#pragma once

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Map.hpp"
#include "mindnet/plugins/slipbox/models/MapCollection.hpp"

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
