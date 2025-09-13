//
// Created by robertvokac on 9/2/25.
//
#ifndef MAPCRUDLVALIDATOR_H
#define MAPCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Map;

    class MapCrudlValidator : public persistence::api::CrudlValidatorBase<MapCrudlValidator, Map>
    {
    public:
        MapCrudlValidator() = default;
        ~MapCrudlValidator() = default; // explicitly make it destructible
        using Model = Map;

        create_method_prototypes_for_CrudlValidatorBase(Map)
    };
}


#endif // MAPCRUDLVALIDATOR_H
