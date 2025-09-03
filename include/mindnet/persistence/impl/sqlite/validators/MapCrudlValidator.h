//
// Created by robertvokac on 9/2/25.
//
#ifndef MAPCRUDLVALIDATOR_H
#define MAPCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidator.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class MapCrudlValidator : public api::CrudlValidator
    {
    public:
        MapCrudlValidator() = default;
        ~MapCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}


#endif // MAPCRUDLVALIDATOR_H
