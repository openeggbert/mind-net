//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/zettelkasten/models/Property.h"

namespace mindnet::plugins::zettelkasten::models
{
    entity_fields Property::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_id);
        result.push_back(note_id);
        result.push_back(key);
        result.push_back(value);
        return result;
    }

    void Property::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_id = number();
        note_id = number();
        key = text();
        value = text();
    };

    string Property::validate()
    {
        using columns::PropertyColumns;

        validator_chain_vector list{
            [this] { return test_ne(map_id, 0, PropertyColumns::MAP_ID); },
            [this] { return test_ne(note_id, 0, PropertyColumns::NOTE_ID); },
            [this] { return testt_between(key, 1, 64, PropertyColumns::KEY); },
        };
        return ValidatorChain::run(list);
    }
}
