//
// Created by robertvokac on 9/2/25.
//
#ifndef NOTENAVIGATIONVALIDATOR_H
#define NOTENAVIGATIONVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/NoteNavigation.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::NoteNavigation;

    class NoteNavigationValidator : public api::ValidatorBase<NoteNavigationValidator, NoteNavigation>
    {
    public:
        NoteNavigationValidator() = default;
        ~NoteNavigationValidator() = default; // explicitly make it destructible
        using Model = NoteNavigation;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // NOTENAVIGATIONVALIDATOR_H
