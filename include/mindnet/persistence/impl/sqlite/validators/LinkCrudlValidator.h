//
// Created by robertvokac on 9/2/25.
//
#ifndef LINKCRUDLVALIDATOR_H
#define LINKCRUDLVALIDATOR_H


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
    using models::Link;

    class LinkCrudlValidator : public api::CrudlValidatorBase<LinkCrudlValidator, models::Link>
    {
    public:
        LinkCrudlValidator() = default;
        ~LinkCrudlValidator() = default; // explicitly make it destructible
        using Model = Link;

        create_method_prototypes_for_CrudlValidatorBase(Link)

    };

}
#endif // LINKCRUDLVALIDATOR_H
