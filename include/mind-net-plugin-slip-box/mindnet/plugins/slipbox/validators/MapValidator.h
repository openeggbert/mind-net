//
// Created by robertvokac on 9/2/25.
//
#ifndef MAPVALIDATOR_H
#define MAPVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"

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

        create_method_prototypes_for_ValidatorBase(Map)
    };
}

#endif // MAPVALIDATOR_H
