//
// Created by robertvokac on 9/2/25.
//
#ifndef QUESTIONCRUDLVALIDATOR_H
#define QUESTIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::Question;

    class QuestionCrudlValidator : public api::CrudlValidatorBase<QuestionCrudlValidator, models::Question>
    {
    public:
        QuestionCrudlValidator() = default;
        ~QuestionCrudlValidator() = default; // explicitly make it destructible
        using Model = Question;

        create_method_prototypes_for_CrudlValidatorBase(Question)

    };

}
#endif // QUESTIONCRUDLVALIDATOR_H
