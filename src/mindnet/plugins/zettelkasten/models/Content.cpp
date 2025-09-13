//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/zettelkasten/models/Content.h"

namespace mindnet::plugins::zettelkasten::models
{
    entity_fields Content::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(value);
        result.push_back(cast64(format));
        result.push_back(version);
        return result;
    }

    void Content::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        value = text();
        format = static_cast<enums::ContentFormat>(number());
        version = number();
    }
    string Content::validate()
    {
        using columns::ContentColumns;

        validator_chain_vector list{
        };
        return ValidatorChain::run(list);
    }
}
