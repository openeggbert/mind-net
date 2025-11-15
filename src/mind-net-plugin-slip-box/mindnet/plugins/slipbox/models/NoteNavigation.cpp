//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/NoteNavigation.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields NoteNavigation::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(prev_note_id);
        result.push_back(next_note_id);

        return result;
    }

    void NoteNavigation::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        prev_note_id = number();
        next_note_id = number();
    }

    string NoteNavigation::validate()
    {
        using columns::NoteNavigationColumns;

        validator_chain_vector list{
        };
        return util::ValidatorChain::run(list);
    }
}
