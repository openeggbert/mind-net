//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/PinnedNote.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
