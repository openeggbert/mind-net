//
// Created by robertvokac on 9/2/25.
//
#ifndef WANTEDNOTEVALIDATOR_H
#define WANTEDNOTEVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "../models/WantedNote.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::WantedNote;

    class WantedNoteValidator : public api::ValidatorBase<
            WantedNoteValidator, WantedNote>
    {
    public:
        WantedNoteValidator() = default;
        ~WantedNoteValidator() = default; // explicitly make it destructible
        using Model = WantedNote;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // WANTEDNOTEVALIDATOR_H
