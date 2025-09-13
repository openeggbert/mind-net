//
// Created by robertvokac on 9/2/25.
//
#ifndef COLLECTIONCRUDLVALIDATOR_H
#define COLLECTIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Collection;

    class CollectionCrudlValidator : public persistence::api::CrudlValidatorBase<CollectionCrudlValidator, Collection>
    {
    public:
        CollectionCrudlValidator() = default;
        ~CollectionCrudlValidator() = default; // explicitly make it destructible
        using Model = Collection;

        create_method_prototypes_for_CrudlValidatorBase(Collection)
    };
}
#endif // COLLECTIONCRUDLVALIDATOR_H
