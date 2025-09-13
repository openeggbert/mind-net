//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONCRUDLVALIDATOR_H
#define COLLECTIONCRUDLVALIDATOR_H


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
    using models::Collection;

    class CollectionCrudlValidator : public api::CrudlValidatorBase<CollectionCrudlValidator, models::Collection>
    {
    public:
        CollectionCrudlValidator() = default;
        ~CollectionCrudlValidator() = default; // explicitly make it destructible
        using Model = Collection;

        create_method_prototypes_for_CrudlValidatorBase(Collection)

    };

}
#endif // COLLECTIONCRUDLVALIDATOR_H
