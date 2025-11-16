//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/R4State.hpp"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::R4State;

    class R4StateValidator : public api::ValidatorBase<R4StateValidator, R4State>
    {
    public:
        R4StateValidator() = default;
        ~R4StateValidator() = default; // explicitly make it destructible
        using Model = R4State;

        create_method_prototypes_for_ValidatorBase(Model)

        // protected:
        //     // Add validation methods for specific R4State fields
        //     bool validateEfTimes100(const Model& model) const
        //     {
        //         return model.ef_times_100 >= 100 && model.ef_times_100 <= 500;
        //     }
        //
        //     bool validateNoteOrQuestionPresent(const Model& model) const
        //     {
        //         return (model.note_id.has_value() || model.question_id.has_value()) &&
        //             !(model.note_id.has_value() && model.question_id.has_value());
        //     }
        //
        //     bool validateIntervalAndRepetitions(const Model& model) const
        //     {
        //         return model.interval >= 1 && model.repetitions >= 0;
        //     }
        //
        //     bool validateQuality(const Model& model) const
        //     {
        //         return model.last_quality >= 0 && model.last_quality <= 5;
        //     }
    };
}
