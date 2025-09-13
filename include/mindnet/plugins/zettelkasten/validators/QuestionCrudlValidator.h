//
// Created by robertvokac on 9/2/25.
//
#ifndef QUESTIONCRUDLVALIDATOR_H
#define QUESTIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Question;

    class QuestionCrudlValidator : public persistence::api::CrudlValidatorBase<QuestionCrudlValidator, Question>
    {
    public:
        QuestionCrudlValidator() = default;
        ~QuestionCrudlValidator() = default; // explicitly make it destructible
        using Model = Question;

        create_method_prototypes_for_CrudlValidatorBase(Question)
    };
}
#endif // QUESTIONCRUDLVALIDATOR_H
