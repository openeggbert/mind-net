//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONITEMCRUDLVALIDATOR_H
#define COLLECTIONITEMCRUDLVALIDATOR_H


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

    class CollectionItemCrudlValidator : public api::CrudlValidator
    {
    public:
        CollectionItemCrudlValidator() = default;
        ~CollectionItemCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // COLLECTIONITEMCRUDLVALIDATOR_H
