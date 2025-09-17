//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/TagType.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields TagType::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_id);
        result.push_back(title);
        return result;
    }

    void TagType::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_id = number();
        title = text();
    };

    string TagType::validate()
    {
        using columns::TagTypeColumns;

        validator_chain_vector list{
            [this] { return test_ne(map_id, 0, TagTypeColumns::MAP_ID); },
            [this] { return testt_between(title, 1, 64, TagTypeColumns::TITLE); },
        };
        return ValidatorChain::run(list);
    }
}
