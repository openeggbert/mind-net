//
// Created by robertvokac on 9/2/25.
//
#ifndef NOTEVALIDATOR_H
#define NOTEVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Note;

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
