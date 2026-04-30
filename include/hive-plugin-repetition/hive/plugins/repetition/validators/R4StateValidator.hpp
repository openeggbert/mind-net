/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <memory>

#include "hive/essential/DatabaseType.hpp"
#include "hive/api/ValidatorBase.hpp"
#include "hive/plugins/repetition/models/R4State.hpp"

namespace hive::plugins::repetition::validators
{
    using api::RequestContext;
    using hive::plugins::repetition::models::R4State;

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