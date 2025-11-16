//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Map.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Map;

    class MapValidator : public api::ValidatorBase<MapValidator, Map>
    {
    public:
        MapValidator() = default;
        ~MapValidator() = default; // explicitly make it destructible
        using Model = Map;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}

