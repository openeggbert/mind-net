//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/Content.h"

namespace mindnet::plugins::slipbox::models
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
        result.push_back(cast64(last_parsed_at));
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
        last_parsed_at = number();
    }

    string Content::validate()
    {
        using columns::ContentColumns;

        validator_chain_vector list{
        };
        return util::ValidatorChain::run(list);
    }
}
