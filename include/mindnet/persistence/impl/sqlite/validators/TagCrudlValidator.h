//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGCRUDLVALIDATOR_H
#define TAGCRUDLVALIDATOR_H


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
    using models::Tag;

    class TagCrudlValidator : public api::CrudlValidatorBase<TagCrudlValidator, models::Tag>
    {
    public:
        TagCrudlValidator() = default;
        ~TagCrudlValidator() = default; // explicitly make it destructible
        using Model = Tag;

        create_method_prototypes_for_CrudlValidatorBase(Tag)

    };

}
#endif // TAGCRUDLVALIDATOR_H
