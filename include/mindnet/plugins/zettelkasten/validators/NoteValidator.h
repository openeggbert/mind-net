//
// Created by robertvokac on 9/2/25.
//
#ifndef NOTEVALIDATOR_H
#define NOTEVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Note;

    class NoteValidator : public api::ValidatorBase<NoteValidator, Note>
    {
    public:
        NoteValidator() = default;
        ~NoteValidator() = default; // explicitly make it destructible
        using Model = Note;

        create_method_prototypes_for_ValidatorBase(Note)
    };
}
#endif // NOTEVALIDATOR_H
