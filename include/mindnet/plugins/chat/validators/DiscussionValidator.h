//
// Created by robertvokac on 9/2/25.
//
#ifndef DISCUSSIONVALIDATOR_H
#define DISCUSSIONVALIDATOR_H

#include <memory>

#include "../../../core/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::chat::validators
{
    using api::RequestContext;
    using mindnet::plugins::chat::models::Discussion;

    class DiscussionValidator : public api::ValidatorBase<DiscussionValidator, Discussion>
    {
    public:
        DiscussionValidator() = default;
        ~DiscussionValidator() = default; // explicitly make it destructible
        using Model = Discussion;

        create_method_prototypes_for_ValidatorBase(Discussion)
    };
}
#endif // DISCUSSIONVALIDATOR_H
