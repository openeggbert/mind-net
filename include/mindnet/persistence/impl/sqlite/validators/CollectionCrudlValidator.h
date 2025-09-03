//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONCRUDLVALIDATOR_H
#define COLLECTIONCRUDLVALIDATOR_H


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

    class CollectionCrudlValidator : public api::CrudlValidator
    {
    public:
        CollectionCrudlValidator() = default;
        ~CollectionCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // COLLECTIONCRUDLVALIDATOR_H
