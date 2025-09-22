//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/slipbox/models/Concept.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Concept::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(cast64(map_id));
        result.push_back(cast64(note_id));
        result.push_back(title);
        result.push_back(disambiguation);

        return result;
    }

    void Concept::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        map_id = number();
        note_id = number();
        title = text();
        disambiguation = text();

    }

    string Concept::validate()
    {
        using columns::ConceptColumns;

        validator_chain_vector list{
            [this] { return testt_between(title, 1, 64, ConceptColumns::TITLE); },
            [this] { return testt_between(disambiguation, 1, 64, ConceptColumns::DISAMBIGUATION); },
            [this] { return test_ne(map_id, 0, ConceptColumns::MAP_ID); },
        };
        return util::ValidatorChain::run(list);
    }
}
