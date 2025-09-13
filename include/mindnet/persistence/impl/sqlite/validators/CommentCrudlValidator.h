//
// Created by robertvokac on 9/2/25.
//
#ifndef COMMENTCRUDLVALIDATOR_H
#define COMMENTCRUDLVALIDATOR_H


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
    using models::Comment;

    class CommentCrudlValidator : public api::CrudlValidatorBase<CommentCrudlValidator, models::Comment>
    {
    public:
        CommentCrudlValidator() = default;
        ~CommentCrudlValidator() = default; // explicitly make it destructible
        using Model = Comment;

        create_method_prototypes_for_CrudlValidatorBase(Comment)

    };

}
#endif // COMMENTCRUDLVALIDATOR_H
