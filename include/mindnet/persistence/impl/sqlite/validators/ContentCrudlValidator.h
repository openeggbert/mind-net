//
// Created by robertvokac on 9/2/25.
//
#ifndef CONTENTCRUDLVALIDATOR_H
#define CONTENTCRUDLVALIDATOR_H


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
    using mindnet::plugins::zettelkasten::models::Content;

    class ContentCrudlValidator : public api::CrudlValidatorBase<ContentCrudlValidator, Content>
    {
    public:
        ContentCrudlValidator() = default;
        ~ContentCrudlValidator() = default; // explicitly make it destructible
        using Model = Content;

        create_method_prototypes_for_CrudlValidatorBase(Content)

    };

}
#endif // CONTENTCRUDLVALIDATOR_H
