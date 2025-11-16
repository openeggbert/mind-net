#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/RReview.hpp"

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

