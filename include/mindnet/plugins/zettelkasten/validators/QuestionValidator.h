//
// Created by robertvokac on 9/2/25.
//
#ifndef QUESTIONVALIDATOR_H
#define QUESTIONVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Question;

    class QuestionValidator : public persistence::api::ValidatorBase<QuestionValidator, Question>
    {
    public:
        QuestionValidator() = default;
        ~QuestionValidator() = default; // explicitly make it destructible
        using Model = Question;

        create_method_prototypes_for_ValidatorBase(Question)
    };
}
#endif // QUESTIONVALIDATOR_H
