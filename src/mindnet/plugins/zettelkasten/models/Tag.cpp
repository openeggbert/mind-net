//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/zettelkasten/models/Tag.h"

namespace mindnet::plugins::zettelkasten::models
{
    entity_fields Tag::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(tag_type_id);
        return result;
    }

    void Tag::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        note_id = number();
        tag_type_id = number();
    };
    string Tag::validate()
    {
        using columns::TagColumns;

        validator_chain_vector list{
            [this] { return test_ne(note_id, 0, TagColumns::NOTE_ID); },
            [this] { return test_ne(tag_type_id, 0, TagColumns::TAG_TYPE_ID); },
        };
        return ValidatorChain::run(list);
    }
}
