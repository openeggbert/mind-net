//
// Created by robertvokac on 9/2/25.
//
#ifndef QUESTIONCRUDLVALIDATOR_H
#define QUESTIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class QuestionCrudlValidator : public api::CrudlValidator
    {
    public:
        QuestionCrudlValidator() = default;
        ~QuestionCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // QUESTIONCRUDLVALIDATOR_H
