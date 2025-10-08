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

        // // Add validation methods for specific fields
        // bool validate_grade(const Model& model) const
        // {
        //     if (model.grade.has_value())
        //     {
        //         int grade = model.grade.value();
        //         return grade >= 0 && grade <= 5;
        //     }
        //     return true;
        // }
        //
        // bool validate_note_question(const Model& model) const
        // {
        //     // Check that either note_id or question_id is provided (not both null)
        //     return model.note_id.has_value() || model.question_id.has_value();
        // }
        //
        // bool validate_timestamps(const Model& model) const
        // {
        //     if (model.started_at.has_value() && model.ended_at.has_value())
        //     {
        //         return model.started_at.value() <= model.ended_at.value();
        //     }
        //     return true;
        // }

        create_method_prototypes_for_ValidatorBase(Model)

    };
}
#endif // RREVIEWVALIDATOR_H
