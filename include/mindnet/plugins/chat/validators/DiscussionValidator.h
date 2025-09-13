//
// Created by robertvokac on 9/2/25.
//
#ifndef DISCUSSIONVALIDATOR_H
#define DISCUSSIONVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::chat::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::chat::models::Discussion;

    class DiscussionValidator : public persistence::api::ValidatorBase<DiscussionValidator, Discussion>
    {
    public:
        DiscussionValidator() = default;
        ~DiscussionValidator() = default; // explicitly make it destructible
        using Model = Discussion;

        create_method_prototypes_for_ValidatorBase(Discussion)
    };
}
#endif // DISCUSSIONVALIDATOR_H
