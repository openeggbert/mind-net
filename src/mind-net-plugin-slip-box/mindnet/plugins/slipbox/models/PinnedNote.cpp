//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/PinnedNote.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields PinnedNote::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(position);
        result.push_back(cast64(is_public));
        return result;
    }

    void PinnedNote::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        position = number();
        is_public = number();
    };

    string PinnedNote::validate()
    {
        using columns::PinnedNoteColumns;

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0, PinnedNoteColumns::USER_ID); },
            [this] { return test_ne(note_id, 0, PinnedNoteColumns::NOTE_ID); },
            [this] { return test_at_least(position, 0, PinnedNoteColumns::POSITION); }
        };
        return util::ValidatorChain::run(list);
    }
}
