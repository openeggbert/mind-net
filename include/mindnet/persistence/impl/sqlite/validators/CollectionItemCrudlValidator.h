//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONITEMCRUDLVALIDATOR_H
#define COLLECTIONITEMCRUDLVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

//namespace mindnet::persistence
//{
//    class Persistence;
//}

namespace mindnet::persistence::impl::sqlite::validators
{
    //using db_ = mindnet::persistence::Persistence*;
    using api::RequestContext;
    using models::CollectionItem;

    class CollectionItemCrudlValidator : public api::CrudlValidatorBase<CollectionItemCrudlValidator, models::CollectionItem>
    {
    public:
        CollectionItemCrudlValidator() = default;
        ~CollectionItemCrudlValidator() = default; // explicitly make it destructible
        using Model = CollectionItem;

        create_method_prototypes_for_CrudlValidatorBase(CollectionItem)

    };

}
#endif // COLLECTIONITEMCRUDLVALIDATOR_H
