//
// Created by robertvokac on 9/2/25.
//
#ifndef PINNEDNOTEVALIDATOR_H
#define PINNEDNOTEVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/PinnedNote.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::PinnedNote;

    class PinnedNoteValidator : public api::ValidatorBase<
            PinnedNoteValidator, PinnedNote>
    {
    public:
        PinnedNoteValidator() = default;
        ~PinnedNoteValidator() = default; // explicitly make it destructible
        using Model = PinnedNote;

        create_method_prototypes_for_ValidatorBase(PinnedNote)
    };
}
#endif // PINNEDNOTEVALIDATOR_H
