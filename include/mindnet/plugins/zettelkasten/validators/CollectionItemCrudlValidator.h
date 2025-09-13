//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONITEMCRUDLVALIDATOR_H
#define COLLECTIONITEMCRUDLVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::CollectionItem;

    class CollectionItemCrudlValidator : public persistence::api::CrudlValidatorBase<CollectionItemCrudlValidator, CollectionItem>
    {
    public:
        CollectionItemCrudlValidator() = default;
        ~CollectionItemCrudlValidator() = default; // explicitly make it destructible
        using Model = CollectionItem;

        create_method_prototypes_for_CrudlValidatorBase(CollectionItem)

    };

}
#endif // COLLECTIONITEMCRUDLVALIDATOR_H
