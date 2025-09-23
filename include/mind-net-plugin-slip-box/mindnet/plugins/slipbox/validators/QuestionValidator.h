//
// Created by robertvokac on 9/2/25.
//
#ifndef QUESTIONVALIDATOR_H
#define QUESTIONVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Question;

    class QuestionValidator : public api::ValidatorBase<QuestionValidator, Question>
    {
    public:
        QuestionValidator() = default;
        ~QuestionValidator() = default; // explicitly make it destructible
        using Model = Question;

        create_method_prototypes_for_ValidatorBase(Question)
    };
}
#endif // QUESTIONVALIDATOR_H
