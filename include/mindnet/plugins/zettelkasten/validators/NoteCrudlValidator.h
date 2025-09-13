//
// Created by robertvokac on 9/2/25.
//
#ifndef NOTECRUDLVALIDATOR_H
#define NOTECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Note;

    class NoteCrudlValidator : public persistence::api::CrudlValidatorBase<NoteCrudlValidator, Note>
    {
    public:
        NoteCrudlValidator() = default;
        ~NoteCrudlValidator() = default; // explicitly make it destructible
        using Model = Note;

        create_method_prototypes_for_CrudlValidatorBase(Note)

    };

}
#endif // NOTECRUDLVALIDATOR_H
