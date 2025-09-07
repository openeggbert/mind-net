//
// Created by robertvokac on 9/2/25.
//
#ifndef NOTECRUDLVALIDATOR_H
#define NOTECRUDLVALIDATOR_H


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
    using models::Note;

    class NoteCrudlValidator : public api::CrudlValidatorBase<NoteCrudlValidator, models::Note>
    {
    public:
        NoteCrudlValidator() = default;
        ~NoteCrudlValidator() = default; // explicitly make it destructible
        using Model = Note;

        create_method_prototypes_for_CrudlValidatorBase(Note)

    };

}
#endif // NOTECRUDLVALIDATOR_H
