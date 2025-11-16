//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
