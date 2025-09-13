//
// Created by robertvokac on 9/2/25.
//
#ifndef COMMENTCRUDLVALIDATOR_H
#define COMMENTCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::chat::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::chat::models::Comment;

    class CommentCrudlValidator : public persistence::api::CrudlValidatorBase<CommentCrudlValidator, Comment>
    {
    public:
        CommentCrudlValidator() = default;
        ~CommentCrudlValidator() = default; // explicitly make it destructible
        using Model = Comment;

        create_method_prototypes_for_CrudlValidatorBase(Comment)

    };

}
#endif // COMMENTCRUDLVALIDATOR_H
