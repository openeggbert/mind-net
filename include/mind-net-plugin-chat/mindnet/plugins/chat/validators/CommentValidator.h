//
// Created by robertvokac on 9/2/25.
//
#ifndef COMMENTVALIDATOR_H
#define COMMENTVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/chat/models/Comment.h"

namespace mindnet::plugins::chat::validators
{
    using api::RequestContext;
    using mindnet::plugins::chat::models::Comment;

    class CommentValidator : public api::ValidatorBase<CommentValidator, Comment>
    {
    public:
        CommentValidator() = default;
        ~CommentValidator() = default; // explicitly make it destructible
        using Model = Comment;

        create_method_prototypes_for_ValidatorBase(Comment)
    };
}
#endif // COMMENTVALIDATOR_H
