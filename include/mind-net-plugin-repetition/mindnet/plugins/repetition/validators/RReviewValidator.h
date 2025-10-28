#ifndef RREVIEWVALIDATOR_H
#define RREVIEWVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/RReview.h"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::RReview;

    class RReviewValidator : public api::ValidatorBase<RReviewValidator, RReview>
    {
    public:
        RReviewValidator() = default;
        ~RReviewValidator() = default;

        using Model = RReview;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // RREVIEWVALIDATOR_H
