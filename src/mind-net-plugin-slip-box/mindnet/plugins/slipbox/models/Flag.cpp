//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/Flag.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Flag::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(title);
        return result;
    }

    void Flag::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        note_id = number();
        title = text();
    };

    string Flag::validate()
    {
        using columns::FlagColumns;

        validator_chain_vector list{
            [this] { return test_ne(note_id, 0, FlagColumns::NOTE_ID); },
            [this] { return testt_not_empty(title, FlagColumns::TITLE); },
        };
        return util::ValidatorChain::run(list);
    }
}
