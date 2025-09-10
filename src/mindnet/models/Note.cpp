//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Note.h"

namespace mindnet::models
{
    entity_fields Note::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_id);
        result.push_back(title);
        result.push_back(parent_note_id);
        result.push_back(content_id);
        result.push_back(sibling_position);
        result.push_back(cast64(importance));
        result.push_back(cast64(difficulty));
        return result;
    }

    void Note::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_id = number();
        title = text();
        parent_note_id = number();
        content_id = number();
        sibling_position = number();
        importance = static_cast<enums::Importance>(number());
        difficulty = static_cast<enums::Difficulty>(number());
    }
    string Note::validate()
    {
        using columns::NoteColumns;

        validator_chain_vector list{
            [this] { return test_ne(map_id, 0, NoteColumns::MAP_ID); },
            [this] { return testt_between(title, 1, 256, NoteColumns::TITLE); },
        };
        return ValidatorChain::run(list);
    }
}
