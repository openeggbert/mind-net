//
// Created by robertvokac on 9/7/25.
//
#ifndef INSTEADOFREADNOTENAVIGATIONTRIGGER_H
#define INSTEADOFREADNOTENAVIGATIONTRIGGER_H


#include "mindnet/api/Trigger.hpp"
#include "mindnet/plugins/slipbox/models/NoteNavigation.hpp"

namespace mindnet::plugins::slipbox::triggers
{
    class InsteadOfReadNoteNavigationTrigger : public api::Trigger
    {
    public:
        InsteadOfReadNoteNavigationTrigger();

        ~InsteadOfReadNoteNavigationTrigger() override = default;
        std::optional<std::pair<entity_fields, api::OperationResult>> run_instead_of_read(
            int stack_depth,
            api::OperationResult& validation_result,
            const model::ModelDefinition& def,
            int user_id,
            int id) override;
    };
}
#endif // INSTEADOFREADNOTENAVIGATIONTRIGGER_H
