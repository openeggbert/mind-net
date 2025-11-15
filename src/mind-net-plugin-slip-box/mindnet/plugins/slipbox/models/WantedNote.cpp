//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/WantedNote.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields WantedNote::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(cast64(from_note_id));
        result.push_back(to_note_title);
        result.push_back(label);

        return result;
    }

    void WantedNote::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        from_note_id = number();
        to_note_title = text();
        label = text();
    }

    string WantedNote::validate()
    {
        using columns::WantedNoteColumns;

        validator_chain_vector list{
            [this] { return test_ne(from_note_id, 0, WantedNoteColumns::FROM_NOTE_ID); },
            [this] { return testt_between(to_note_title, 1, 256, WantedNoteColumns::TO_NOTE_TITLE); },
        };
        return util::ValidatorChain::run(list);
    }
}
